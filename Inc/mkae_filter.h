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

#ifndef MKAE_FILTER_H_
#define MKAE_FILTER_H_

#include "mkAE_functions.h"

//#define MAHONY
#define MASUYA

void mkAE_Filter_Init(double cycle_s);
void mkAE_IMU_MS2016_SetParam_Test(mkAE_IMU_MS2016 *mkae_ms);
void mkAE_UpdateAngle(double *yaw,double *pitch,double *roll);
void Get_SENS_VAL(double *p_gyro,double *p_acc);
double Get_Temp(void);

void Get_mes(double *gyro, double *acc);

void Get_RawData(double *gyro_raw, double *acc_raw);
void Get_HexData(u32 *gyro_hex, u32 *acc_hex);

void Get_SensSensi(double *gyro_sensi, double *acc_sensi);

void mkAE_Reset_Filter();

#endif /* MKAE_FILTER_H_ */
