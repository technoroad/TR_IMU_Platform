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

#ifndef IMU_INC_ADIS_ERROR_H_
#define IMU_INC_ADIS_ERROR_H_

#define ADIS1647X_CLOCKERROR 0x0080
#define ADIS1647X_MEMORYERROR 0x0040
#define ADIS1647X_SENSORFAILURE 0x0020
#define ADIS1647X_STANDBYMODE 0x0010
#define ADIS1647X_SPICOMMERROR 0x0008
#define ADIS1647X_MEMORYUPDATEFAILURE 0x0004
#define ADIS1647X_DATAPATHOVERRUN 0x0002

#define ADIS1649X_WATCHDOGTIMER 0x0100
#define ADIS1649X_SYNCERROR 0x0080
#define ADIS1649X_PROCESSINGOVERRUN 0x0040
#define ADIS1649X_MEMORYUPDATEFAILURE 0x0020
#define ADIS1649X_SENSORFAILURE 0x0010
#define ADIS1649X_SPICOMMERROR 0x0008
#define ADIS1649X_SRAMERROR 0x0004
#define ADIS1649X_BOOTMEMORYFAILURE 0x0002

#endif /* IMU_INC_ADIS_ERROR_H_ */
