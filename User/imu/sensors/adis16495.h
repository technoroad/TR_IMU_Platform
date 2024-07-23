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

#ifndef INC_ADIS16495_H_
#define INC_ADIS16495_H_

# if defined(ADIS16495)

// Delay per data of SPI communication [us]
#define TSTALL 5

#if defined(_1BMLZ)
#define GYRO_SENSITIVITY (double)10485760.0
#define ACCL_SENSITIVITY (double)262144000.0
#define GYRO_NOISE_DENSITY 0.002
#elif defined(_2BMLZ)
#define GYRO_SENSITIVITY (double)2621440.0
#define ACCL_SENSITIVITY (double)262144000.0
#define GYRO_NOISE_DENSITY 0.0022
#elif defined(_3BMLZ)
#define GYRO_SENSITIVITY (double)655360.0
#define ACCL_SENSITIVITY (double)262144000.0
#define GYRO_NOISE_DENSITY 0.0042
#endif

#define BURST_CMD 0x7C
#define BURST_CMD_SIZE 21

#define PRODUCT_ID 0x406F
#define CONVERSION_RATE 4250
#define MAX_TBC 13

//==============================================================================
// PAGE 0 address
//==============================================================================
#define PAGE_ID 0
#define DATA_CNT 0x04
#define SYS_E_FLAG 0x08
#define TEMP_OUT 0x0E
#define PROD_ID 0x7E
#define Z_ACCL_OUT 0x26

//==============================================================================
// PAGE 2 address
//==============================================================================
#define XG_BIAS_LOW 0x10
#define ZG_BIAS_HIGH 0x1A
#define ZA_BIAS_HIGH 0x26

//==============================================================================
// PAGE 3 address
//==============================================================================
#define GLOB_CMD 0x02
#define MSC_CTRL 0x0A
#define DEC_RATE 0x0C
#define NULL_CNFG 0x0E
#define FILTR_BNK_0 0x16
#define FILTR_BNK_1 0x18
#define FIRM_REV 0x78


#define PAGE0 0
#define PAGE1 1
#define PAGE2 2
#define PAGE3 3

#define FILT_BANK_A 0
#define FILT_BANK_B 1
#define FILT_BANK_C 2
#define FILT_BANK_D 3
#define FILTER_OFF (int)-1

#endif

#endif /* INC_ADIS16495_H_ */
