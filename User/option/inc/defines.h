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

#ifndef INC_DEFINES_H_
#define INC_DEFINES_H_

typedef void (*UserEventHandlerPtr)(void);
typedef volatile uint8_t bool;

#define true 1
#define false 0

#define CSUM_SIZE 2

//Standard Gravity Acceleration
#define STANDARD_GRAVITY 9.80665

#define ADIS1647X_CLOCKERROR 0x0080
#define ADIS1647X_MEMORYERROR 0x0040
#define ADIS1647X_SENSORFAILURE 0x0020
#define ADIS1647X_STANDBYMODE 0x0010
#define ADIS1647X_SPICOMMERROR 0x0008
#define ADIS1647X_MEMORYUPDATEFAILURE 0x0004
#define ADIS1647X_DATAPATHOVERRUN 0x0002

// List of character output formats
typedef enum {
  kADI_DRIVER = 0b0000,
  kYawPitchRoll = 0b0001,
  kGYRO_Degree = 0b0010,
  kGYRO_ACC_HexData = 0b0011,
  kGYRO_ACC_BinaryData = 0b0100,
  kYawPitchRoll_ACC = 0b0101,
  kGYRO_ACC_TEMP = 0b0110,
  kPose_GYRO_ACC_CNT = 0b0111,
  kUsbToUart = 0b1111,
} StringFormat;

// List of system status
typedef enum {
  kStartup,
  kStartupWait,
  kUpdatePose,
  kAdiDriver,
  kFatalError,
  kXportSetting,
} SystemStatus;

typedef enum {
  IMU_Type1 = 1,
  IMU_Type2 = 2,
  IMU_Type3 = 3,
} IMU_Type;

typedef struct{
  IMU_Type type;
  double gyro_sensi;
  double accl_sensi;
  double gyro_noise_density;
  uint8_t tstall;
  uint16_t conversion_rate;
  uint8_t dec_rate;
  double dt;
  uint8_t max_tbc;
  uint8_t filter;
  bool burst_enable;
  uint8_t burst_cmd;
  uint8_t burst_cmd_size;
  uint16_t product_id;
}IMU_TypeSetting;

// Struct of parameters to be saved in flash
typedef struct __attribute__((aligned(2), packed)){
  double gain_p;
  double gain_i;
  uint32_t transmit_prescaler;
  uint32_t startup_time;  // [s]
  uint32_t auto_update_enable;
  uint32_t auto_update_time;
  uint32_t stationary_observe_enable;
  double stational_accl_thresh;
  double stational_gyro_thresh;
} IMU_Config;

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

typedef struct {
  void (*SetDecimationRate)(void);
  void (*EnableBurstRead)(void);
  void (*BiasCorrectionUpdate)(void);
  void (*SetHardwareFilter)(uint8_t f0);
  void (*SetBiasCorrectionTime)(uint32_t sec);
  uint16_t (*GetDiagnosticFlag)(void);
  uint16_t (*GetProductId)(void);
  uint16_t (*GetRangeModel)(void);
  void (*WriteData)(uint8_t page, uint8_t addr, uint16_t data);
  uint16_t (*ReadData)(uint8_t page, uint16_t addr);
  void (*Update6AxisDataWithSingleRead)(void);
  void (*Update6AxisDataWithBurstRead)(void);
  void (*UpdateGyroBias)(void);
} IMU_Function;

typedef struct {
  IMU_Config conf;
  IMU_TypeSetting tset;
  IMU_Function func;
  ImuDataList data;
} System_Status;

typedef struct __attribute__((aligned(2), packed)){
  IMU_Config conf;
  uint32_t version;
  uint16_t csum;
} Flash_Settings;

#endif /* INC_DEFINES_H_ */
