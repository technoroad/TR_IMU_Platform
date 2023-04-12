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

// Struct of parameters to be saved in flash
typedef volatile struct {
  uint32_t version;
  double gain_p;
  double gain_i;
  uint32_t transmit_prescaler;
  uint32_t startup_time;  // [s]
  uint32_t auto_average_time;  // [s]
  double vibration_threshold;  // [m/s^2]
  uint32_t auto_average_enable;
  uint16_t csum;
} __attribute__((__aligned__(2)))__attribute__((packed)) BoardParameterList;

#endif /* INC_DEFINES_H_ */
