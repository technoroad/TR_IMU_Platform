/*
 The MIT License (MIT)

 Copyright (c) 2023 Techno Road Inc.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

/* Includes ------------------------------------------------------------------*/
#include "libraries.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
// Save raw data of angular velocity and acceleration
static ImuDataList kIMU = { 0 };

/* Private function prototypes -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/**
 * @brief  Returns a pointer to the IMU data structure.
 */
ImuDataList* IMU_GetDataList() {
  return &kIMU;
}

/*
 * @brief  Get sensor sensitivity
 */
void IMU_GetSensitivity(double *gyro_sensi, double *accl_sensi) {
  *gyro_sensi = GYRO_SENSITIVITY;
  *accl_sensi = ACCL_SENSITIVITY;
}

/**
 * @brief  Store the data read by IMU_UpdateGyroBias() in the IMU data structure.
 */
void IMU_ConvertBiasData(uint16_t *data) {
  for (int i = 0; i < 3; i++) {
    kIMU.gyro_bias.raw[i] = ((data[i * 2 + 1] << 16) | data[i * 2]);

    kIMU.gyro_bias.fpp[i] =  //
        (int32_t) kIMU.gyro_bias.raw[i] / GYRO_SENSITIVITY * M_PI / 180.0;
  }
}

/**
 * @brief  Returns whether it is a burst read from the page and address.
 * @note   The page works only for the ADIS1649X series.
 */
bool IMU_IsBurstReadCommand(uint8_t page, uint16_t addr) {
#if defined(ADIS1647X)
  if (addr == BURST_CMD) {
    return true;
  }
#elif defined(ADIS1649X)
  if (page == PAGE0 && addr == BURST_CMD) {
    return true;
  }
#endif
  return false;
}

#if defined(ADIS1647X)
/**
 * @brief  Store the data read by
 *         IMU_Update6AxisDataWithSingleRead() in the IMU data structure.
 */
void IMU_Convert32bitDataWithSingleRead(uint16_t *data) {
  kIMU.diag_stat = data[0];
  for (int i = 0; i < 3; i++) {
    kIMU.gyro.raw[i] = ((data[i * 2 + 2] << 16) | data[i * 2 + 1]);
    kIMU.accl.raw[i] = ((data[i * 2 + 8] << 16) | data[i * 2 + 7]);

    kIMU.gyro.fpp[i] =  //
        (int32_t) kIMU.gyro.raw[i] / GYRO_SENSITIVITY * M_PI / 180.0;

    kIMU.accl.fpp[i] = (int32_t) kIMU.accl.raw[i] / ACCL_SENSITIVITY;
  }
  kIMU.temperature = (double) ((int16_t) data[13]) * 0.1;
  kIMU.data_cnt = data[14];
}

/**
 * @brief  Store the data read by
 *         IMU_Update6AxisDataWithSingleRead() in the IMU data structure.
 */
void IMU_Convert32bitDataWithBurstRead(uint16_t *data) {
  kIMU.diag_stat = data[0];
  for (int i = 0; i < 3; i++) {
    kIMU.gyro.raw[i] = ((data[i * 2 + 2] << 16) | data[i * 2 + 1]);
    kIMU.accl.raw[i] = ((data[i * 2 + 8] << 16) | data[i * 2 + 7]);

    kIMU.gyro.fpp[i] =  //
        (int32_t) kIMU.gyro.raw[i] / GYRO_SENSITIVITY * M_PI / 180.0;

    kIMU.accl.fpp[i] = (int32_t) kIMU.accl.raw[i] / ACCL_SENSITIVITY;
  }
  kIMU.temperature = (double) ((int16_t) data[13]) * 0.1;
  kIMU.data_cnt = data[14];

  uint32_t sum = 0;
  for (int i = 0; i < 15; i++) {
    sum += data[i] & 0xff;
    sum += (data[i] >> 8) & 0xff;
  }

  kIMU.sum = sum;
  kIMU.csum = data[15];
}
#elif defined(ADIS1649X)
/**
 * @brief  Store the data read by
 *         IMU_Update6AxisDataWithSingleRead() in the IMU data structure.
 */
void IMU_Convert32bitDataWithSingleRead(uint16_t *data) {
  kIMU.diag_stat = data[0];
  for (int i = 0; i < 3; i++) {
    kIMU.gyro.raw[i] = ((data[i * 2 + 2] << 16) | data[i * 2 + 1]);
    kIMU.accl.raw[i] = ((data[i * 2 + 8] << 16) | data[i * 2 + 7]);

    kIMU.gyro.fpp[i] =  //
        (int32_t) kIMU.gyro.raw[i] / GYRO_SENSITIVITY * M_PI / 180.0;

    kIMU.accl.fpp[i] = (int32_t) kIMU.accl.raw[i] / ACCL_SENSITIVITY;
  }
  kIMU.temperature = 25.0 + (double) ((int16_t) data[5]) / 80.0;
  kIMU.data_cnt = data[14];
}

/**
 * @brief  Store the data read by
 *         IMU_Update6AxisDataWithSingleRead() in the IMU data structure.
 */
void IMU_Convert32bitDataWithBurstRead(uint16_t *data) {
  // Since the data rarely deviates, it is corrected.
  if (data[1] == 0xA5A5) {
    data = &data[1];
  }
  // once again.
  if (data[1] == 0xA5A5) {
    data = &data[1];
  }

  kIMU.diag_stat = data[1];
  for (int i = 0; i < 3; i++) {
    kIMU.gyro.raw[i] = ((data[i * 2 + 4] << 16) | data[i * 2 + 3]);
    kIMU.accl.raw[i] = ((data[i * 2 + 10] << 16) | data[i * 2 + 9]);

    kIMU.gyro.fpp[i] =  //
        (int32_t) kIMU.gyro.raw[i] / GYRO_SENSITIVITY * M_PI / 180.0;

    kIMU.accl.fpp[i] = (int32_t) kIMU.accl.raw[i] / ACCL_SENSITIVITY;
  }
  kIMU.temperature = 25.0 + (double) ((int16_t) data[2]) / 80.0;
  kIMU.data_cnt = data[15];
  kIMU.recv_crc32 = ((data[17] << 16) | data[16]);
  kIMU.calc_crc32 = ~(MakeCrc32(&data[1],15));
}
#endif
