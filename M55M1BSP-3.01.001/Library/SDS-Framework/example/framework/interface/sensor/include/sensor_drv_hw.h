/*
 * Copyright (c) 2022 Arm Limited. All rights reserved.
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

/* Nuvoton CyChen update 3/3 */

#ifndef SENSOR_DRV_HW_H
#define SENSOR_DRV_HW_H

#ifdef  __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "sensor_drv.h"

// ==== Sensor Driver Hardware Interface ====

// Function documentation
/**
  \fn          int32_t sensorDrvHW_RegisterEvents (sensorId_t id, sensorEvent_t event_cb, uint32_t event_mask)
  \brief       Register sensor events.
  \param[in]   id          \ref sensorId_t
  \param[in]   event_cb    pointer to \ref sensorEvent_t
  \param[in]   event_mask  event mask
  \return      return code
*/
/**
  \fn          int32_t sensorDrvHW_Enable (void)
  \brief       Enable sensor.
  \return      return code
*/
/**
  \fn          int32_t sensorDrvHW_Disable (void)
  \brief       Disable sensor.
  \return      return code
*/
/**
  \fn          uint32_t sensorDrvHW_GetOverflow (void)
  \brief       Get overflow status.
  \return      0 = no overflow, 1 = overflow (automatically cleared)
*/
/**
  \fn          uint32_t sensorDrvHW_ReadSamples (uint32_t num_samples, void *buf)
  \brief       Read samples from sensor.
  \param[in]   num_samples maximum number of samples to read
  \param[out]  buf         pointer to buffer for samples
  \return      number of samples read
*/
/**
  \fn          void *sensorDrvHW_GetBlockData (void)
  \brief       Get block data.
  \return      pointer to block data
*/

/// Functions
typedef const struct
{
    int32_t (*RegisterEvents)(sensorId_t id, sensorEvent_t event_cb, uint32_t event_mask);
    int32_t (*Enable)(void);
    int32_t (*Disable)(void);
    uint32_t (*GetOverflow)(void);
    uint32_t (*ReadSamples)(uint32_t num_samples, void *buf);
    uint32_t (*ReadTwoSensorsSamples)(uint32_t num_samples, void *buf1,  void *buf2);
    void    *(*GetBlockData)(void);
} sensorDrvHW_t;

extern sensorDrvHW_t sensorDrvHW_0;
extern sensorDrvHW_t sensorDrvHW_1;
extern sensorDrvHW_t sensorDrvHW_2;
extern sensorDrvHW_t sensorDrvHW_3;
extern sensorDrvHW_t sensorDrvHW_4;
extern sensorDrvHW_t sensorDrvHW_5;
extern sensorDrvHW_t sensorDrvHW_6;
extern sensorDrvHW_t sensorDrvHW_7;

#ifdef  __cplusplus
}
#endif

#endif  /* SENSOR_DRV_HW_H */
