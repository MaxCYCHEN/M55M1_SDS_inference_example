/*
 * Copyright (c) 2023 Arm Limited. All rights reserved.
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

// SDS I/O interface via File System (Keil::File System)

#include <string.h>
#include <stdio.h>

#include "rl_fs.h"                      // Keil.MDK-Plus::File System:CORE
#include "sdsio.h"
#include "sdsio_config_fs_mdk.h"


// Maximum stream name length
#ifndef SDSIO_MAX_NAME_SIZE
#define SDSIO_MAX_NAME_SIZE         32U
#endif

// Max length of index and file extension
#define SDSIO_MAX_EXT_SIZE          16U

// SDS I/O functions

/** Initialize I/O interface */
int32_t sdsioInit (void) {
  int32_t  ret = SDSIO_ERROR;
  uint32_t stat;

  // Initialize and mount file system drive
  stat = finit(SDSIO_DRIVE);
  if (stat == fsOK) {
    stat = fmount(SDSIO_DRIVE);
#if (SDSIO_FORMATTING_ALLOWED == 1)
    if (stat == fsNoFileSystem) {
      stat = fformat(SDSIO_DRIVE, NULL);
    }
#endif
  }

  if (stat == fsOK) {
    ret = SDSIO_OK;
  }
  return ret;
}

/** Un-initialize I/O interface */
int32_t sdsioUninit (void) {
  funmount(SDSIO_DRIVE);
  funinit(SDSIO_DRIVE);
  return SDSIO_OK;
}

/** Open I/O stream */
sdsioId_t sdsioOpen (const char *name, sdsioMode_t mode) {
  uint32_t   index   = 0U;
  sdsioId_t  sdsioId = NULL;
  FILE      *file    = NULL;
  char       file_name[sizeof(SDSIO_WORK_DIR) + SDSIO_MAX_NAME_SIZE + SDSIO_MAX_EXT_SIZE];
  char       line[16];

  if (strlen(name) <= SDSIO_MAX_NAME_SIZE) {
    switch (mode) {
      case sdsioModeRead:

        sprintf(file_name, "%s%s.index.txt", SDSIO_WORK_DIR, name);
        file = fopen(file_name, "r");
        if (file != NULL) {
					printf("Open read index file\n");
          if (fscanf(file, "%i", &index) != 1) {
            index = 0U;
          }
          fclose(file);
        }

        sprintf(file_name, "%s%s.%i.sds", SDSIO_WORK_DIR, name, index);
        file = fopen(file_name, "rb");
        if (file != NULL) {
					printf("Open read sds file\n");
          // File exists
          index++;
          sdsioId = (sdsioId_t)file;
        } else {
          if (index != 0U) {
            index = 0U;
          }
        }

        if ((sdsioId != NULL) || (index == 0U)) {
          sprintf(file_name, "%s%s.index.txt", SDSIO_WORK_DIR, name);
          file = fopen(file_name, "w");
          if (file != NULL) {
						printf("Open write index file\n");
            sprintf(line, "%i\r\n", index);
            fwrite(line, 1, strlen(line) + 1U, file);
            fclose(file);
          }
        }
        break;
      case sdsioModeWrite:
        while (sdsioId == NULL) {
          sprintf(file_name, "%s%s.%i.sds", SDSIO_WORK_DIR, name, index);
          file = fopen(file_name, "rb");
          if (file != NULL) {
            // File already exists
            fclose(file);
            index++;
          } else {
            file = fopen(file_name, "wb");
            sdsioId = (sdsioId_t)file;
            break;
          }
        }
        break;
    }
  }
  return sdsioId;
}

/** Close I/O stream. */
int32_t sdsioClose (sdsioId_t id) {
  int32_t ret  = SDSIO_ERROR;
  FILE   *file = (FILE *)id;

  if (fclose(file) == 0) {
    ret = SDSIO_OK;
  }
  return ret;
}

/** Write data to I/O stream. */
uint32_t sdsioWrite (sdsioId_t id, const void *buf, uint32_t buf_size) {
  FILE *file = (FILE *)id;
  return fwrite(buf, 1, buf_size, file);
}

/** Read data from I/O stream. */
uint32_t sdsioRead (sdsioId_t id, void *buf, uint32_t buf_size) {
  FILE *file = (FILE *)id;
  return fread(buf, 1, buf_size, file);
}

/** Check if end of stream has been reached. */
int32_t sdsioEndOfStream (sdsioId_t id) {
  FILE *file = (FILE *)id;
  return feof(file);
}
