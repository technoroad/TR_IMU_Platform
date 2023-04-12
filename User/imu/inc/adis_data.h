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

#ifndef IMU_INC_ADIS_DATA_H_
#define IMU_INC_ADIS_DATA_H_

typedef struct {
  uint32_t raw[3];
  double fpp[3];
} ThreeAxisData;

typedef volatile struct {
  uint16_t diag_stat;
  ThreeAxisData gyro;
  ThreeAxisData accl;
  double temperature;
  uint16_t data_cnt;
  double accl_synthesis;
  double vibration_peak;
  ThreeAxisData gyro_bias;
  double yaw;
  double pitch;
  double roll;
  uint16_t csum;
  uint16_t sum;
  uint32_t recv_crc32;
  uint32_t calc_crc32;
} ImuDataList;

void IMU_Convert32bitDataWithSingleRead(uint16_t *data);
void IMU_Convert32bitDataWithBurstRead(uint16_t *data);
void IMU_ConvertBiasData(uint16_t *data);
ImuDataList* IMU_GetDataList();
void IMU_GetSensitivity(double *GyroSensi, double *AccSensi);
bool IMU_IsBurstReadCommand(uint8_t page, uint16_t addr);

#endif /* IMU_INC_ADIS_DATA_H_ */
