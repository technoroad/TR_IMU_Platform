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

#ifndef INITIAL_PARAMETER_H_
#define INITIAL_PARAMETER_H_

#include <libraries.h>

#define VERSION 0x20240826    // Version
#define GAIN_P_INIT 1.0    // Kp gain
#define GAIN_I_INIT 0.0    // Ki gain
#define TRANSMIT_PRESCALER_INIT 0  //
#define VIBRATION_THRESHOLD_INIT 0.1
#define MOTION_THRESHOLD_INIT 0.1
#define AUTO_UPDATE_ENABLE_INIT 0
#define AUTO_UPDATE_TIME_INIT 5
#define ST_OBSERVE_ENABLE_INIT 0
#define ST_ACCL_INIT 0.05
#define ST_GYRO_INIT 0.05

// Standby counter at startup
#if defined(ADIS16470)
#define STARTUP_TIME_INIT 120;        // 120[s]
#elif defined(ADIS16475)
#define STARTUP_TIME_INIT 120;        // 120[s]
#elif defined(ADIS16477)
#define STARTUP_TIME_INIT 120;        // 120[s]
#elif defined(ADIS16495)
#define STARTUP_TIME_INIT 60;        // 60[s]
#elif defined(ADIS16500)
#define STARTUP_TIME_INIT 120;        // 120[s]
#elif defined(ADIS16505)
#define STARTUP_TIME_INIT 120;        // 120[s]
#endif

void InitializeParameters();

#endif /* INITIAL_PARAMETER_H_ */
