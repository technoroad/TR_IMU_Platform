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
#define BUF_SIZE 256

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static char kSendBuf[BUF_SIZE];
static StringFormat kStringFormat = kYawPitchRoll;
static bool kSendEnableFlg = false;

/* Private function prototypes -----------------------------------------------*/
void OutputStringType1(double *array, int length, int ap);
void OutputStringType2(uint32_t *array, int length);
void OutputStringType3(double *a0, int a0_l, int ap, uint32_t *a1, int a1_l);
void OutputBinary(uint32_t *array, int length);
void PushString(void);

/* Global variables ----------------------------------------------------------*/
extern BoardParameterList gBoard;

/**
 * @brief  IMU data is stored in the transmission buffer,
 *         but the transmission rate is changed by "transmit_prescaler".
 */
void PushImuData() {
  // Output according to the set transmission period.
  static int cnt = 0;
  if (kSendEnableFlg) {
    if (cnt < gBoard.transmit_prescaler) {
      cnt++;
    } else {
      PushString();
      cnt = 0;
    }
  } else {
    cnt = 0;
  }
}

/**
 * @brief  Returns the data transmission status.
 */
bool IsSendEnable(void) {
  return kSendEnableFlg;
}

/**
 * @brief  Start transmission imu data.
 */
void SendStart() {
  kSendEnableFlg = true;
}

/**
 * @brief  Stop transmission imu data.
 */
void SendStop() {
  kSendEnableFlg = false;
}

/**
 * @brief  Format the data.
 */
void SetOutputFormat(StringFormat format) {
  kStringFormat = format;
}

/**
 * @brief  Returns the data format.
 */
StringFormat GetOutputFormat(void) {
  return kStringFormat;
}

/**
 * @brief  Update and transmit attitude angles.
 */
void PushString(void) {
  ImuDataList *list = IMU_GetDataList();

  // The mode is branched by the CONF switch.
  switch (kStringFormat) {

    // Attitude angle output mode
    case kYawPitchRoll: {
      double arr[3];
      arr[0] = list->yaw;
      arr[1] = list->pitch;
      arr[2] = list->roll;

      OutputStringType1(arr, 3, 3);
      break;
    }

      // Output mode of angular velocity
    case kGYRO_Degree: {
#if 0
      double arr[6];
      arr[0] = list->gyro.fpp[0];
      arr[1] = list->gyro.fpp[1];
      arr[2] = list->gyro.fpp[2];
      arr[3] = list->gyro.fpp[0] - list->gyro_bias.fpp[0];
      arr[4] = list->gyro.fpp[1] - list->gyro_bias.fpp[1];
      arr[5] = list->gyro.fpp[2] - list->gyro_bias.fpp[2];

      OutputStringType1(arr, 6,6);
#else
      OutputStringType1((double*) list->gyro.fpp, 3, 3);
#endif
      break;
    }

      // This mode transmits raw data of sensor values as a character string.
    case kGYRO_ACC_HexData: {
      uint32_t arr[6];
      arr[0] = list->gyro.raw[0];
      arr[1] = list->gyro.raw[1];
      arr[2] = list->gyro.raw[2];
      arr[3] = list->accl.raw[0];
      arr[4] = list->accl.raw[1];
      arr[5] = list->accl.raw[2];

      OutputStringType2(arr, 6);
      break;
    }

      // Mode to transmit raw data of sensor values in binary.
    case kGYRO_ACC_BinaryData: {
      uint32_t arr[6];
      arr[0] = list->gyro.raw[0];
      arr[1] = list->gyro.raw[1];
      arr[2] = list->gyro.raw[2];
      arr[3] = list->accl.raw[0];
      arr[4] = list->accl.raw[1];
      arr[5] = list->accl.raw[2];

      OutputBinary(arr, 6);
      break;
    }

      // Mode to transmit attitude angle and acceleration values.
    case kYawPitchRoll_ACC: {
      double arr[6];
      arr[0] = list->yaw;
      arr[1] = list->pitch;
      arr[2] = list->roll;
      arr[3] = list->accl.fpp[0];
      arr[4] = list->accl.fpp[1];
      arr[5] = list->accl.fpp[2];

      OutputStringType1(arr, 6, 3);
      break;
    }

    case kGYRO_ACC_TEMP: {
      double arr[7];
      arr[0] = list->gyro.fpp[0];
      arr[1] = list->gyro.fpp[1];
      arr[2] = list->gyro.fpp[2];
      arr[3] = list->accl.fpp[0];
      arr[4] = list->accl.fpp[1];
      arr[5] = list->accl.fpp[2];
      arr[6] = list->temperature;

      OutputStringType1(arr, 7, 3);
      break;
    }

    case kPose_GYRO_ACC_CNT: {
      double a0[9];
      uint32_t a1;
      a0[0] = list->yaw;
      a0[1] = list->pitch;
      a0[2] = list->roll;
      a0[3] = list->gyro.fpp[0];
      a0[4] = list->gyro.fpp[1];
      a0[5] = list->gyro.fpp[2];
      a0[6] = list->accl.fpp[0];
      a0[7] = list->accl.fpp[1];
      a0[8] = list->accl.fpp[2];
      a1 = list->data_cnt;
      OutputStringType3(a0, 9, 3, &a1, 1);
    }

    default: {
      break;
    }
  }
}

/**
 * @brief  Send the 16-bits register in CSV format.
 */
void OutputStringType2(uint32_t *array, int length) {
  kSendBuf[0] = 0;
  for (int i = 0; i < length; i++) {
    str_concat(kSendBuf, str_putx(array[i], 8));

    str_concat(kSendBuf, ",");
  }

  uint32_t sum = 0;
  for (int i = 0; i < length; i++) {
    sum += (array[i] >> 24) & 0xff;
    sum += (array[i] >> 16) & 0xff;
    sum += (array[i] >> 8) & 0xff;
    sum += (array[i]) & 0xff;
  }

  str_concat(kSendBuf, str_putx(sum & 0xff, 2));

  str_concat(kSendBuf, "\r\n\0");
  COMM_puts(kSendBuf);
}

/**
 * @brief  Send data in CSV format.
 */
void OutputStringType1(double *array, int length, int ap) {
  kSendBuf[0] = 0;
  for (int i = 0; i < length; i++) {
    str_concat(kSendBuf, str_putlf(array[i], ap));

    if (i < (length - 1)) {
      str_concat(kSendBuf, ",");
    }
  }
  str_concat(kSendBuf, "\r\n\0");
  COMM_puts(kSendBuf);
}

/**
 * @brief  Send data in binary fomat.
 */
void OutputBinary(uint32_t *array, int length) {
  uint8_t counter = 0;
  kSendBuf[counter++] = 0xAC;
  kSendBuf[counter++] = 0xCA;
  kSendBuf[counter++] = 24;
  for (int i = 0; i < 6; i++) {
    kSendBuf[counter++] = (array[i] >> 24) & 0xff;
    kSendBuf[counter++] = (array[i] >> 16) & 0xff;
    kSendBuf[counter++] = (array[i] >> 8) & 0xff;
    kSendBuf[counter++] = (array[i]) & 0xff;
  }

  uint32_t sum = 0;
  for (int i = 2; i < counter; i++) {
    sum += kSendBuf[i];
  }
  kSendBuf[counter++] = sum & 0xff;
  COMM_putd(kSendBuf, (char) counter);
}

/**
 * @brief  Send data in CSV format3.
 */
void OutputStringType3(double *a0, int a0_l, int ap, uint32_t *a1, int a1_l) {
  kSendBuf[0] = 0;
  for (int i = 0; i < a0_l; i++) {
    str_concat(kSendBuf, str_putlf(a0[i], ap));

    str_concat(kSendBuf, ",");
  }

  for (int i = 0; i < a1_l; i++) {
    str_concat(kSendBuf, str_putn2(a1[i]));

    if (i < (a1_l - 1)) {
      str_concat(kSendBuf, ",");
    }
  }

  str_concat(kSendBuf, "\r\n\0");
  COMM_puts(kSendBuf);
}
