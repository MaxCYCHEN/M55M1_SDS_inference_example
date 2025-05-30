/*
 * Copyright (c) 2022-2024 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// SDS Recorder

#include <stdatomic.h>
#include <string.h>
#include <stdio.h>

#include "cmsis_compiler.h"
#include "cmsis_os2.h"
#include "sds.h"
#include "sdsio.h"
#include "sds_rec.h"
#include "sds_rec_config.h"

#if SDS_REC_MAX_STREAMS > 31
#error "Maximum number of SDS Recorder streams is 31!"
#endif

// Control block
typedef struct {
           uint8_t     index;
           uint8_t     event_threshold;
           uint8_t     event_close;
           uint8_t     flags;
           uint32_t    buf_size;
           sdsId_t     stream;
           sdsioId_t   sdsio;
} sdsRec_t;

static sdsRec_t   RecStreams[SDS_REC_MAX_STREAMS] = {0};
static sdsRec_t *pRecStreams[SDS_REC_MAX_STREAMS] = {NULL};

// Record header
typedef struct {
  uint32_t    timestamp;        // Timestamp in ticks
  uint32_t    data_size;        // Data size in bytes
} RecHead_t;

// Record buffer
static uint8_t RecBuf[SDS_REC_BUF_SIZE];
//static float RecBuf[SDS_REC_BUF_SIZE];

// Event callback
static sdsRecEvent_t sdsRecEvent = NULL;

// Thread Id
static osThreadId_t sdsRecThreadId;

// Close event flags
static osEventFlagsId_t sdsRecCloseEventFlags;

// Event definitions
#define SDS_REC_EVENT_FLAG_MASK ((1UL << SDS_REC_MAX_STREAMS) - 1)

// Flag definitions
#define SDS_REC_FLAG_CLOSE      (1U << 0)

// Helper functions

// Atomic Operation: Write 32-bit value to memory, if existing value in memory is zero
//  Return: 1 when new value is written or 0 otherwise
#if ATOMIC_CHAR32_T_LOCK_FREE < 2
__STATIC_INLINE uint32_t atomic_wr32_if_zero (uint32_t *mem, uint32_t val) {
  uint32_t primask = __get_PRIMASK();
  uint32_t ret = 0U;

  __disable_irq();
  if (*mem == 0U) {
    *mem = val;
    ret = 1U;
  }
  if (primask == 0U) {
    __enable_irq();
  }

  return ret;
}
#else
__STATIC_INLINE uint32_t atomic_wr32_if_zero (uint32_t *mem, uint32_t val) {
  uint32_t expected;
  uint32_t ret = 1U;

  expected = *mem;
  do {
    if (expected != 0U) {
      ret = 0U;
      break;
    }
  } while (!atomic_compare_exchange_weak_explicit((_Atomic uint32_t *)mem,
                                                  &expected,
                                                  val,
                                                  memory_order_acq_rel,
                                                  memory_order_relaxed));

  return ret;
}
#endif

static sdsRec_t * sdsRecAlloc (uint32_t *index) {
  sdsRec_t *rec = NULL;
  uint32_t  n;

  for (n = 0U; n < SDS_REC_MAX_STREAMS; n++) {
    if (atomic_wr32_if_zero((uint32_t *)&pRecStreams[n], (uint32_t)&RecStreams[n]) != 0U) {
      rec = &RecStreams[n];
      if (index != NULL) {
        *index = n;
      }
      break;
    }
  }
  return rec;
}

static void sdsRecFree (uint32_t index) {
  pRecStreams[index] = NULL;
}

// Event callback
static void sdsRecEventCallback (sdsId_t id, uint32_t event, void *arg) {
  uint32_t  index = (uint32_t)arg;
  sdsRec_t *rec;
  (void)id;
  (void)event;

  rec = pRecStreams[index];
  if (rec != NULL) {
    rec->event_threshold = 1U;
  }
}

// Recorder thread
static __NO_RETURN void sdsRecThread (void *arg) {
  sdsRec_t *rec;
  uint32_t  flags, cnt, n;

  (void)arg;

  while (1) {
    flags = osThreadFlagsWait(SDS_REC_EVENT_FLAG_MASK, osFlagsWaitAny, osWaitForever);
    if ((flags & osFlagsError) == 0U) {
      for (n = 0U; n < SDS_REC_MAX_STREAMS; n++) {
        if ((flags & (1U << n)) == 0U) {
          continue;
        }
        rec = pRecStreams[n];
        if (rec == NULL) {
          continue;
        }
        if ((rec->flags & SDS_REC_FLAG_CLOSE) != 0U) {
          continue;
        }

        cnt = sdsGetCount(rec->stream);
        while (cnt != 0U) {
          if (cnt > sizeof(RecBuf)) {
            cnt = sizeof(RecBuf);
          }
          sdsRead(rec->stream, RecBuf, cnt);
					
	  uint32_t write_cnt = sdsioWrite(rec->sdsio, RecBuf, cnt);
	  printf("sdsRecThread cnt: %u, write_cnt: %u \n", cnt, write_cnt);
          if (write_cnt != cnt) {
            if (sdsRecEvent != NULL) {
              sdsRecEvent(rec, SDS_REC_EVENT_IO_ERROR);
            }
            break;
          }
          cnt = sdsGetCount(rec->stream);
        }

        if (rec->event_close != 0U) {
          rec->flags |= SDS_REC_FLAG_CLOSE;
          osEventFlagsSet(sdsRecCloseEventFlags, 1U << rec->index);
        }
      }
    }
  }
}

// SDS Recorder functions

// Initialize recorder
int32_t sdsRecInit (sdsRecEvent_t event_cb) {
  int32_t ret = SDS_REC_ERROR;

  memset(pRecStreams, 0, sizeof(pRecStreams));

  if (sdsioInit() == SDSIO_OK) {
    sdsRecThreadId = osThreadNew(sdsRecThread, NULL, NULL);
    if (sdsRecThreadId != NULL)  {
      sdsRecCloseEventFlags = osEventFlagsNew(NULL);
      if (sdsRecCloseEventFlags != NULL) {
        sdsRecEvent = event_cb;
        ret = SDS_REC_OK;
      }
    }
  }
  return ret;
}

// Uninitialize recorder
int32_t sdsRecUninit (void) {
  int32_t ret = SDS_REC_ERROR;

  osThreadTerminate(sdsRecThreadId);
  osEventFlagsDelete(sdsRecCloseEventFlags);
  sdsRecEvent = NULL;
  sdsioUninit();

  return ret;
}

// Open recorder stream
sdsRecId_t sdsRecOpen (const char *name, void *buf, uint32_t buf_size, uint32_t io_threshold) {
  sdsRec_t *rec = NULL;
  uint32_t  index;

  if ((name != NULL) && (buf != NULL) && (buf_size != 0U) &&
      (buf_size <= SDS_REC_BUF_SIZE) && (io_threshold <= buf_size)) {

    rec = sdsRecAlloc(&index);
    if (rec != NULL) {
      rec->index           = index & 0xFFU;
      rec->event_threshold = 0U;
      rec->event_close     = 0U;
      rec->flags           = 0U;
      rec->buf_size        = buf_size;
      rec->stream          = sdsOpen(buf, buf_size, 0U, io_threshold);
      rec->sdsio           = sdsioOpen(name, sdsioModeWrite);

      if (rec->stream != NULL) {
        sdsRegisterEvents(rec->stream, sdsRecEventCallback, SDS_EVENT_DATA_HIGH, (void *)index);
	printf("rec->stream set\n");
      }
      if ((rec->stream == NULL) || (rec->sdsio == NULL)) {
        if (rec->stream != NULL) {
          sdsClose(rec->stream);
          rec->stream = NULL;
	  //		printf("rec->stream clean\n");
        }
        if (rec->sdsio != NULL) {
          sdsioClose(rec->sdsio);
          rec->sdsio = NULL;
	  //		printf("rec->sdsio clean\n");
        }
        sdsRecFree(index);
        rec = NULL;
      }
    }
  }
  return rec;
}

// Close recorder stream
int32_t sdsRecClose (sdsRecId_t id) {
  sdsRec_t *rec = id;
  int32_t   ret = SDS_REC_ERROR;
  uint32_t  event_mask;

  if (rec != NULL) {
    event_mask =  1 << rec->index;
    rec->event_close = 1U;
    osThreadFlagsSet(sdsRecThreadId, event_mask);
    osEventFlagsWait(sdsRecCloseEventFlags, event_mask, osFlagsWaitAll, osWaitForever);

    sdsClose(rec->stream);
    sdsioClose(rec->sdsio);
    sdsRecFree(rec->index);

    ret = SDS_REC_OK;
  }
  return ret;
}

// Write data to recorder stream
uint32_t sdsRecWrite (sdsRecId_t id, uint32_t timestamp, const void *buf, uint32_t buf_size) {
  sdsRec_t *rec = id;
  uint32_t  num = 0U;
  RecHead_t rec_head;

  if ((rec != NULL) && (buf != NULL) && (buf_size != 0U)) {
    if ((buf_size + sizeof(RecHead_t)) <= (rec->buf_size -  sdsGetCount(rec->stream))) {
      // Write record to the stream: timestamp, data size, data
      rec_head.timestamp = timestamp;
      rec_head.data_size = buf_size;
      if (sdsWrite(rec->stream, &rec_head, sizeof(RecHead_t)) == sizeof(RecHead_t)) {
        num = sdsWrite(rec->stream, buf, buf_size);
        if (num == buf_size) {
          if (rec->event_threshold != 0U) {
            rec->event_threshold = 0U;
            osThreadFlagsSet(sdsRecThreadId, 1U << rec->index);
          }
        }
      }
    }
  }
  return num;
}
