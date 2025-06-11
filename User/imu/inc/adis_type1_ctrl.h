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

#ifndef INC_ADIS_TYPE1_CTRL_H_
#define INC_ADIS_TYPE1_CTRL_H_

// ADIS16470、ADIS16475など
#define TYPE1_DIAG_STAT     0x02
#define TYPE1_X_GYRO_LOW    0x04
#define TYPE1_Z_ACCL_OUT    0x1A
#define TYPE1_TEMP_OUT      0x1C
#define TYPE1_DATA_CNT      0x22
#define TYPE1_XG_BIAS_LOW   0x40
#define TYPE1_ZG_BIAS_HIGH  0x4A
#define TYPE1_FILT_CTRL     0x5C
#define TYPE1_RANG_MDL      0x5E
#define TYPE1_MSC_CTRL      0x60
#define TYPE1_DEC_RATE      0x64
#define TYPE1_NULL_CNFG     0x66
#define TYPE1_GLOB_CMD      0x68
#define TYPE1_FIRM_REV      0x6C
#define TYPE1_PROD_CMD      0x72

#define TYPE1_BURST_CMD     0x68

void IMU_RegisterType1Functions();

#endif /* INC_ADIS_TYPE1_CTRL_H_ */
