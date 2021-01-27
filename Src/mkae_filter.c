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

#include <libraries.h>
#include "math.h"
#include "string.h"

//Control cycle[s]
double DT =0.001;

double temperature =0;
double acc0[3];

#ifdef MAHONY
	mkAE_IMU mkae_Mah;
	double Kp_Mah=1, Ki_Mah=0.1;
	double wA_Mah=1;
	double ang_Mah[3];
#elif defined(MASUYA)
	mkAE_IMU_MS2016 mkae_ms;
//	double Kp_ms=1, Ki_ms=0.1;
	double wA_ms=1;
	#define freqGT 100.0
	#define freqAcc 3.0
	#define freqGyro 30.0
	#define freqRelax 90.0
#endif

extern FLASH_ROM_Parameters Params;

/* Sensor dynamics for the test */
void mkAE_IMU_MS2016_SetParam_Test(mkAE_IMU_MS2016 *mkae_ms){
#ifdef MASUYA
	int i;
	/* Filter for the estimate of the acceleration */
	for(i=0;i<3;i++) mk_Set_Param_1stOrderFilter_LP(mkae_ms->param_EstAcc0Fil[i], 1.0, freqAcc, DT);

	/* Filter for the accelerometer's measurement */
//	for(i=0;i<3;i++) *mkae_ms->param_MesAccFil[i] =1.0;
	for(i=0;i<3;i++) mk_Set_Param_1stOrderFilter_LP(mkae_ms->param_MesAccFil[i], 1.0, freqRelax, DT);

	/* Filter for the gyroscope's measurement */
//	for(i=0;i<3;i++) *mkae_ms->param_MesgyroFil[i] =1.0;
	for(i=0;i<3;i++) mk_Set_Param_1stOrderFilter_PC(mkae_ms->param_MesgyroFil[i], 1.0, freqGyro, freqRelax, DT);
#endif
}

// cycle_s:Please put the update cycle of the attitude angle.
// unit[s].
void mkAE_Filter_Init(double cycle_s){
	double gyro[3], acc[3];

	Get_SENS_VAL(gyro,acc);

	for(int i=0;i<3;i++){
		acc0[i] =acc[i];
	}

	DT = cycle_s;

#ifdef MAHONY
  mkAE_IMU_Initialize(&mkae_Mah, acc);

#elif defined(MASUYA)
  // Filter initialization
//  mkAE_MARG_MS2016_Initialize(&mkae_ms, gyro, acc, mag, 1, 0, 1, 0, 0);
  mkAE_IMU_MS2016_Initialize(&mkae_ms, gyro, acc, 1, 1, 1);
  mkAE_IMU_MS2016_SetParam_Test(&mkae_ms);
#endif
}

#ifdef MAHONY
static double gyro_mah[3], acc_mah[3];
#endif

//yaw : Rotation angle of Z axis
//pitch : Rotation angle of Y axis
//roll : Rotation angle of X axis
void mkAE_UpdateAngle(double *yaw,double *pitch,double *roll){
	double gyro[3], acc[3];

	//Get angular velocity and angular velocity
	Get_SENS_VAL(gyro,acc);

#ifdef MAHONY
	// Output of pose matrix
	mkAE_IMU_MahonyECF_AM(&mkae_Mah, gyro, acc, wA_Mah, Kp_Mah, Ki_Mah, DT);
	// Output of Euler angles from pose matrix
	mk_Cmpt_ZYXangle_AM(mkae_Mah.m_att, ang_Mah);

	// Output of yaw pitch roll from pose matrix
	*yaw=ang_Mah[0]*180.0/M_PI;
	*pitch =ang_Mah[1]*180.0/M_PI;
	*roll =ang_Mah[2]*180.0/M_PI;

	// Save value only for MAHONY
	for(int i =0;i<3;i++){
		gyro_mah[i] = gyro[i];
		acc_mah[i] = acc[i];
	}

#elif defined(MASUYA)
	double ang_MS2016[3];

	// Output of pose matrix
	mkAE_IMU_MS2016_CF_AM(&mkae_ms, gyro, acc, wA_ms, Params.gain_p, Params.gain_i, DT);
	// Output of Euler angles from pose matrix
	mk_Cmpt_ZYXangle_AM(mkae_ms.mkae.m_att, ang_MS2016);

	// Output of yaw pitch roll from pose matrix
	*yaw=ang_MS2016[0]*180.0/M_PI;
	*pitch =ang_MS2016[1]*180.0/M_PI;
	*roll =ang_MS2016[2]*180.0/M_PI;
#endif
}

// Set the output values of yaw, pitch and roll to 0.
void mkAE_Reset_Filter(){
	double gyro[3],acc[3];
	Get_mes(gyro,acc);

#ifdef MAHONY
	mkAE_IMU_Reset(&mkae_Mah, gyro, acc);
#elif defined(MASUYA)
	mkAE_IMU_MS2016_Reset(&mkae_ms, gyro, acc);
#endif
}

// Calculate angular velocity and acceleration values.
void Get_SENS_VAL(double *p_gyro,double *p_acc){
	uint16_t rbuf[18];
	memset(rbuf,0,sizeof(rbuf));

	// Get value
	//ADIS_32bit_READ(rbuf);
	ADIS_32bit_BURST_READ(rbuf);

	// Calculation of angular velocity and acceleration
	ADIS_32bitIMUConvert(rbuf,p_gyro,p_acc);

	//Temperature (not used for pose angle)
	temperature =ADIS_16bitTempConvert(rbuf);
}

double Get_Temp(void){
	return temperature;
}

// Get sensor value stored in mkae_ms
void Get_mes(double *gyro, double *acc){
#ifdef MAHONY
	for(int i=0;i<3;i++){
		gyro[i] = gyro_mah[i];
		acc[i] = acc_mah[i];
	}
#elif defined(MASUYA)
	for(int i=0;i<3;i++){
		gyro[i] = *mkae_ms._gyro_mes[i];
		acc[i] = *mkae_ms._acc_mes[i];
	}
#endif
}

//Get hex data
void Get_HexData(u32 *gyro_hex, u32 *acc_hex){
	ADIS_GetHexData(gyro_hex,acc_hex);
}

//Get raw data
void Get_RawData(double *gyro_raw, double *acc_raw){
	ADIS_GetRawData(gyro_raw,acc_raw);
}

// Get sensor sensitivity
void Get_SensSensi(double *gyro_sensi, double *acc_sensi){
	ADIS_Get_SensSensi(gyro_sensi,acc_sensi);
}
