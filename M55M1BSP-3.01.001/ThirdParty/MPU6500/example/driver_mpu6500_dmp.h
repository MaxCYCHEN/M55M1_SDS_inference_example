/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      driver_mpu6500_dmp.h
 * @brief     driver mpu6500 dmp header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2024-07-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/07/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_MPU6500_DMP_H
#define DRIVER_MPU6500_DMP_H

#include "driver_mpu6500_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup mpu6500_example_driver
 * @{
 */

/**
 * @brief mpu6500 dmp example default definition
 */
#define MPU6500_DMP_DEFAULT_CLOCK_SOURCE                   MPU6500_CLOCK_SOURCE_PLL                  /**< pll */
#define MPU6500_DMP_DEFAULT_RATE                           50                                        /**< 50Hz */
#define MPU6500_DMP_DEFAULT_ACCELEROMETER_RANGE            MPU6500_ACCELEROMETER_RANGE_2G            /**< 2g */
#define MPU6500_DMP_DEFAULT_GYROSCOPE_RANGE                MPU6500_GYROSCOPE_RANGE_2000DPS           /**< 2000dps */
#define MPU6500_DMP_DEFAULT_LOW_PASS_FILTER                MPU6500_LOW_PASS_FILTER_3                 /**< low pass filter 3 */
#define MPU6500_DMP_DEFAULT_CYCLE_WAKE_UP                  MPU6500_BOOL_FALSE                        /**< disable cycle wake up */
#define MPU6500_DMP_DEFAULT_INTERRUPT_PIN_LEVEL            MPU6500_PIN_LEVEL_LOW                     /**< low level */
#define MPU6500_DMP_DEFAULT_INTERRUPT_PIN_TYPE             MPU6500_PIN_TYPE_PUSH_PULL                /**< push pull */
#define MPU6500_DMP_DEFAULT_INTERRUPT_MOTION               MPU6500_BOOL_TRUE                         /**< enable motion */
#define MPU6500_DMP_DEFAULT_INTERRUPT_FIFO_OVERFLOW        MPU6500_BOOL_TRUE                         /**< enable fifo overflow */
#define MPU6500_DMP_DEFAULT_INTERRUPT_DMP                  MPU6500_BOOL_FALSE                        /**< disable dmp */
#define MPU6500_DMP_DEFAULT_INTERRUPT_FSYNC_INT            MPU6500_BOOL_FALSE                        /**< disable fsync int */
#define MPU6500_DMP_DEFAULT_INTERRUPT_DATA_READY           MPU6500_BOOL_FALSE                        /**< disable data ready */
#define MPU6500_DMP_DEFAULT_INTERRUPT_LATCH                MPU6500_BOOL_TRUE                         /**< enable latch */
#define MPU6500_DMP_DEFAULT_INTERRUPT_READ_CLEAR           MPU6500_BOOL_TRUE                         /**< enable interrupt read clear */
#define MPU6500_DMP_DEFAULT_EXTERN_SYNC                    MPU6500_EXTERN_SYNC_INPUT_DISABLED        /**< extern sync input disable */
#define MPU6500_DMP_DEFAULT_FSYNC_INTERRUPT                MPU6500_BOOL_FALSE                        /**< disable fsync interrupt */
#define MPU6500_DMP_DEFAULT_FSYNC_INTERRUPT_LEVEL          MPU6500_PIN_LEVEL_LOW                     /**< low level */
#define MPU6500_DMP_DEFAULT_IIC_MASTER                     MPU6500_BOOL_FALSE                        /**< disable iic master */
#define MPU6500_DMP_DEFAULT_IIC_BYPASS                     MPU6500_BOOL_FALSE                        /**< disable iic bypass */
#define MPU6500_DMP_DEFAULT_PEOMETER_WALK_TIME             200                                       /**< 200ms */
#define MPU6500_DMP_DEFAULT_PEOMETER_STEP_COUNT            0                                         /**< 0 */
#define MPU6500_DMP_DEFAULT_SHAKE_REJECT_TIMEOUT           10                                        /**< 10ms */
#define MPU6500_DMP_DEFAULT_SHAKE_REJECT_TIME              40                                        /**< 40ms */
#define MPU6500_DMP_DEFAULT_SHAKE_REJECT_THRESH            200                                       /**< 200dps */
#define MPU6500_DMP_DEFAULT_TAP_TIME_MULTI                 200                                       /**< 200ms */
#define MPU6500_DMP_DEFAULT_TAP_TIME                       100                                       /**< 100ms */
#define MPU6500_DMP_DEFAULT_MIN_TAP_COUNT                  1                                         /**< 1 */
#define MPU6500_DMP_DEFAULT_TAP_X_THRESH                   250                                       /**< 250 mg/ms */
#define MPU6500_DMP_DEFAULT_TAP_Y_THRESH                   250                                       /**< 250 mg/ms */
#define MPU6500_DMP_DEFAULT_TAP_Z_THRESH                   250                                       /**< 250 mg/ms */
#define MPU6500_DMP_DEFAULT_INTERRUPT_MODE                 MPU6500_DMP_INTERRUPT_MODE_CONTINUOUS     /**< interrupt continuous mode */
#define MPU6500_DMP_DEFAULT_MOTION_THRESHOLD               200                                       /**< 200 mg */
#define MPU6500_DMP_DEFAULT_GYROSCOPE_STANDBY              MPU6500_BOOL_FALSE                        /**< disable gyro standby */
#define MPU6500_DMP_DEFAULT_FIFO_MODE                      MPU6500_FIFO_MODE_NORMAL                  /**< normal mode */
#define MPU6500_DMP_DEFAULT_GYROSCOPE_CHOICE               0                                         /**< 0 */
#define MPU6500_DMP_DEFAULT_ACCELEROMETER_CHOICE           0                                         /**< 0 */
#define MPU6500_DMP_DEFAULT_ACCELEROMETER_LOW_PASS_FILTER  MPU6500_ACCELEROMETER_LOW_PASS_FILTER_3   /**< low pass filter 3 */
#define MPU6500_DMP_DEFAULT_LOW_POWER_ACCEL_OUTPUT_RATE    MPU6500_LOW_POWER_ACCEL_OUTPUT_RATE_62P50 /**< 62.5Hz */
#define MPU6500_DMP_DEFAULT_ACCELEROMETER_COMPARE          MPU6500_BOOL_TRUE                         /**< enable compare */
#define MPU6500_DMP_DEFAULT_MAGNETOMETER_MODE              MPU6500_MAGNETOMETER_MODE_CONTINUOUS2     /**< 100Hz */
#define MPU6500_DMP_DEFAULT_MAGNETOMETER_BITS              MPU6500_MAGNETOMETER_BITS_16              /**< 16bits */
#define MPU6500_DMP_DEFAULT_IIC_CLOCK                      MPU6500_IIC_CLOCK_400_KHZ                 /**< 400KHz */
#define MPU6500_DMP_DEFAULT_IIC_MULTI_MASTER               MPU6500_BOOL_TRUE                         /**< enable multi master */
#define MPU6500_DMP_DEFAULT_IIC_WAIT_FOR_EXTERNAL_SENSOR   MPU6500_BOOL_FALSE                        /**< disable wait for external sensor */
#define MPU6500_DMP_DEFAULT_IIC_READ_MODE                  MPU6500_IIC_READ_MODE_RESTART             /**< restart mode */
#define MPU6500_DMP_DEFAULT_IIC_DELAY                      MPU6500_BOOL_FALSE                        /**< disable iic delay */

/**
 * @brief  dmp irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t mpu6500_dmp_irq_handler(void);

/**
 * @brief     dmp example init
 * @param[in] interface used interface
 * @param[in] addr_pin iic device address
 * @param[in] *receive_callback pointer to a receive callback function
 * @param[in] *tap_callback pointer to a tap callback function
 * @param[in] *orient_callback pointer to an orient callback function
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t mpu6500_dmp_init(mpu6500_interface_t interface, mpu6500_address_t addr_pin,
                         void (*receive_callback)(uint8_t type),
                         void (*tap_callback)(uint8_t count, uint8_t direction),
                         void (*orient_callback)(uint8_t orientation)
                        );

/**
 * @brief  dmp example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t mpu6500_dmp_deinit(void);

/**
 * @brief         dmp example read
 * @param[out]    *accel_raw pointer to an accel raw buffer
 * @param[out]    *accel_g pointer to an accel g buffer
 * @param[out]    *gyro_raw pointer to a gyro raw buffer
 * @param[out]    *gyro_dps pointer to a gyro dps buffer
 * @param[out]    *quat pointer to a quat buffer
 * @param[out]    *pitch pointer to a pitch buffer
 * @param[out]    *roll pointer to a roll buffer
 * @param[out]    *yaw pointer to a yaw buffer
 * @param[in,out] *l pointer to a length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 * @note          none
 */
uint8_t mpu6500_dmp_read_all(int16_t (*accel_raw)[3], float (*accel_g)[3],
                             int16_t (*gyro_raw)[3], float (*gyro_dps)[3],
                             int32_t (*quat)[4],
                             float *pitch, float *roll, float *yaw,
                             uint16_t *l
                            );

/**
 * @brief      dmp example get pedometer counter
 * @param[out] *cnt pointer to a cnt buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pedometer counter failed
 * @note       none
 */
uint8_t mpu6500_dmp_get_pedometer_counter(uint32_t *cnt);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
