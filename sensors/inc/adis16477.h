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

#ifndef INC_ADIS16477_H_
#define INC_ADIS16477_H_

#include "def.h"
#include "select_sensor.h"

# if defined(ADIS16477)

#define PRODUCT_ID 0x405D

#define FLASH_CNT   	0x00  //Flash memory write count
#define DIAG_STAT   	0x02  //Diagnostic and operational status
#define X_GYRO_LOW  	0x04  //X-axis gyroscope output, lower word
#define X_GYRO_OUT  	0x06  //X-axis gyroscope output, upper word
#define Y_GYRO_LOW  	0x08  //Y-axis gyroscope output, lower word
#define Y_GYRO_OUT  	0x0A  //Y-axis gyroscope output, upper word
#define Z_GYRO_LOW  	0x0C  //Z-axis gyroscope output, lower word
#define Z_GYRO_OUT  	0x0E  //Z-axis gyroscope output, upper word
#define X_ACCL_LOW  	0x10  //X-axis accelerometer output, lower word
#define X_ACCL_OUT  	0x12  //X-axis accelerometer output, upper word
#define Y_ACCL_LOW  	0x14  //Y-axis accelerometer output, lower word
#define Y_ACCL_OUT  	0x16  //Y-axis accelerometer output, upper word
#define Z_ACCL_LOW  	0x18  //Z-axis accelerometer output, lower word
#define Z_ACCL_OUT  	0x1A  //Z-axis accelerometer output, upper word
#define TEMP_OUT    	0x1C  //Temperature output (internal, not calibrated)
#define TIME_STAMP  	0x1E  //PPS mode time stamp
#define X_DELTANG_LOW	0x24  //X-axis delta angle output, lower word
#define X_DELTANG_OUT	0x26  //X-axis delta angle output, upper word
#define Y_DELTANG_LOW	0x28  //Y-axis delta angle output, lower word
#define Y_DELTANG_OUT	0x2A  //Y-axis delta angle output, upper word
#define Z_DELTANG_LOW	0x2C  //Z-axis delta angle output, lower word
#define Z_DELTANG_OUT	0x2E  //Z-axis delta angle output, upper word
#define X_DELTVEL_LOW	0x30  //X-axis delta velocity output, lower word
#define X_DELTVEL_OUT	0x32  //X-axis delta velocity output, upper word
#define Y_DELTVEL_LOW	0x34  //Y-axis delta velocity output, lower word
#define Y_DELTVEL_OUT	0x36  //Y-axis delta velocity output, upper word
#define Z_DELTVEL_LOW	0x38  //Z-axis delta velocity output, lower word
#define Z_DELTVEL_OUT	0x3A  //Z-axis delta velocity output, upper word
#define XG_BIAS_LOW		0x40  //X-axis gyroscope bias offset correction, lower word
#define XG_BIAS_HIGH	0x42  //X-axis gyroscope bias offset correction, upper word
#define YG_BIAS_LOW		0x44  //Y-axis gyroscope bias offset correction, lower word
#define YG_BIAS_HIGH	0x46  //Y-axis gyroscope bias offset correction, upper word
#define ZG_BIAS_LOW		0x48  //Z-axis gyroscope bias offset correction, lower word
#define ZG_BIAS_HIGH	0x4A  //Z-axis gyroscope bias offset correction, upper word
#define XA_BIAS_LOW		0x4C  //X-axis accelerometer bias offset correction, lower word
#define XA_BIAS_HIGH	0x4E  //X-axis accelerometer bias offset correction, upper word
#define YA_BIAS_LOW		0x50  //Y-axis accelerometer bias offset correction, lower word
#define YA_BIAS_HIGH	0x52  //Y-axis accelerometer bias offset correction, upper word
#define ZA_BIAS_LOW		0x54  //Z-axis accelerometer bias offset correction, lower word
#define ZA_BIAS_HIGH	0x56  //Z-axis accelerometer bias offset correction, upper word
#define FILT_CTRL    	0x5C  //Filter control
#define MSC_CTRL    	0x60  //Miscellaneous control
#define UP_SCALE    	0x62  //Clock scale factor, PPS mode
#define DEC_RATE    	0x64  //Decimation rate control (output data rate)
#define NULL_CFG    	0x66  //Auto-null configuration control
#define GLOB_CMD    	0x68  //Global commands
#define FIRM_REV    	0x6C  //Firmware revision
#define FIRM_DM    		0x6E  //Firmware revision date, month and day
#define FIRM_Y    		0x70  //Firmware revision date, year
#define PROD_ID    		0x72  //Product identification
#define SERIAL_NUM    0x74  //Serial number (relative to assembly lot)
#define USER_SCR1    	0x76  //User scratch register 1
#define USER_SCR2    	0x78  //User scratch register 2
#define USER_SCR3    	0x7A  //User scratch register 3
#define FLSHCNT_LOW   0x7C  //Flash update count, lower word
#define FLSHCNT_HIGH  0x7E  //Flash update count, upper word

#define BURST_CMD 0x68	//burst command

//ADIS_HardwareFilterSelect
#define FILTER_OFF (int)-1

void ADIS_INIT(void);
void ADIS_Bias_Correction_Update(void);
void ADIS_32bit_READ(uint16_t *rb);

void ADIS_WRITE_REG(u16 addr,u16 value);
u16 ADIS_READ_REG(u16 addr);
u16 ADIS_PAGE_DUMP(u16 * vals);
bool ADIS_HardwareFilterSelect(int data);

int ADIS_32bitIMUConvert(u16 *buf,double *gyro,double *acc);
void ADIS_GetHexData(u32 *p_gyro, u32 *p_acc);
void ADIS_GetRawData(double *p_gyro, double *p_acc);
double ADIS_16bitTempConvert(u16 *buf);

u16 ADIS_Bloking_RegRead(u8 addr);
u16 ADIS_NoBloking_RegRead(u8 addr);
int ADIS_RegWrite_8bit(u8 regAddr, s8 regData);
int ADIS_RegWrite_16bit(u8 regAddr, s16 regData);

u8 ADIS_VerificationProductId(void);

void ADIS_Get_SensSensi(double *GyroSensi,double *AccSensi);

#endif

#endif /* INC_ADIS16477_H_ */
