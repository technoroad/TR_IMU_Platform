/*
The MIT License (MIT)

Copyright (c) 2019 Techno Road Inc.

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

#ifndef ADIS16495_H_
#define ADIS16495_H_

#include "def.h"
#include "select_sensor.h"

# if defined(ADIS16495)

#define PRODUCT_ID 0x406F

#define PAGE_ID 0
#define PAGE0_BURST_CMD 0x7C
#define PAGE0_PROD_ID 0x7E
#define PAGE3_GLOB_CMD 0x02
#define PAGE3_FILTR_BNK_0 0x16
#define PAGE3_FILTR_BNK_1 0x18

#define PAGE0 0
#define PAGE1 1
#define PAGE2 2
#define PAGE3 3

#define FILT_BANK_A 0
#define FILT_BANK_B 1
#define FILT_BANK_C 2
#define FILT_BANK_D 3
#define FILTER_OFF (int)-1

void ADIS_INIT(void);
void ADIS_Bias_Correction_Update(void);
void ADIS_32bit_READ(uint16_t *rb);

void ADIS_WRITE_REG(u8 page,u16 addr,u16 value);
u16 ADIS_READ_REG(u8 page,u16 addr);
u16 ADIS_PAGE_DUMP(u8 page,u16 * vals);
bool ADIS_HardwareFilterSelect(int gx_f, int gy_f, int gz_f, int ax_f, int ay_f, int az_f);

int ADIS_32bitIMUConvert(u16 *buf,double *gyro,double *acc);
void ADIS_GetHexData(u32 *p_gyro, u32 *p_acc);
void ADIS_GetRawData(double *p_gyro, double *p_acc);
double ADIS_16bitTempConvert(u16 *buf);

u16 ADIS_Bloking_RegRead(u8 addr);
u16 ADIS_NoBloking_RegRead(u8 addr);
int ADIS_RegWrite(u8 regAddr, s16 regData);

u8 ADIS_VerificationProductId(void);
void ADIS_BIAS_READ(u32 *bias);

void ADIS_Get_SensSensi(double *GyroSensi,double *AccSensi);

#endif

#endif /* ADIS16495_H_ */
