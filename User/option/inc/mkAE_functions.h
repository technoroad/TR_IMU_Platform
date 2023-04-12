/*
The MIT License (MIT)

Copyright (c) 2019 Ken Masuya

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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*****************************************************************************************************/
/* Computation of the 3-D matrix multiplication, m3 = m1.m2 */
void mk_Cmpt_AM_AM(double m1[][3], double m2[][3], double m3[][3]);
/* Computation of the product of the 3-D matrix and 3-D vector, v2 = m1.v1 */
void mk_Cmpt_AM_Vec(double m1[][3], double *v1, double *v2);
/* Computation of the product of the 3-D matrix (transposed) and 3-D vector, v2 = m1^T.v1 */
void mk_Cmpt_AMT_Vec(double m1[][3], double *v1, double *v2);
/* Computation of the outer product of the 3-D vector, v3 = v1 times v2 */
void mk_Cmpt_OuterProduct(double *v1, double *v2, double *v3);


/* Computation of Z-Y-X Euler angle from the attitude matrix */
void mk_Cmpt_ZYXangle_AM(double m_att[][3], double *ang);
/* Computation of the attitude matrix from Z-Y-X Euler angle */
void mk_Cmpt_AM_ZYXangle(double *ang, double m_att[][3]);
/* Computation of Z-Y-X Euler angle from the quaternion */
void mk_Cmpt_ZYXangle_Quat(double *q, double *ang);
/* Computation of the quaternion from Z-Y-X Euler angle */
void mk_Cmpt_Quat_ZYXangle(double *ang, double *q);
/* Computation of the attitude matrix from the quaternion */
void mk_Cmpt_AM_Quat(double *q, double m_att[][3]);
/* Computation of the quaternion from the attitude matrix */
void mk_Cmpt_Quat_AM(double m_att[][3], double *q);

/* Computation of the matrix based on the Rodrigues's rotation formula */
void mk_Cmpt_AM_Rodrigues(double m[][3], double *v);
/* Computation of the vector rotated by the quaternion, v2 = R(q1).v1 */
void mk_Cmpt_Rotate_Quat_Vec(double *q1, double *v1, double *v2);
/* Computation of the quaternion derivative, q2 = (1/2)*q1*gyro */
void mk_Cmpt_Quat_Derivative(double *q1, double *gyro, double *q2);


/* Filter functions
 * The arguments of the functions are as follows:
 *	- a_mes   : Current measurement
 *	- a_mes   : Measurement of the previous times
 *	- a_est_m : Estimate of the previous times
 *	- param   : Filter parameters
 *	- a_est   : Current estimate (output)
 *
 * mk_Cmpt_1stOrderFilter() computes by the 1st-order filter
 * mk_Cmpt_2ndOrderFilter() computes the result by the 2nd-order filter 
 * mk_Cmpt_012_OrderFilter() computes the result by the 0th-order, 1st-order, 2nd-order filter. 
 */
void mk_Cmpt_1stOrderFilter(double *a_mes, double *a_mes_m, double *a_est_m, double *param, double *a_est);
void mk_Cmpt_2ndOrderFilter(double *a_mes, double *a_mes_m, double *a_est_m, double *param, double *a_est);
void mk_Cmpt_012_OrderFilter(int n_order, double *a_mes, double *a_mes_m, double *a_est_m, double *param, double *a_est);

/* Allocate the filter
 * The argument of those functions is as follows:
 *	- n_order : Order of the filter 
 *
 * mk_Alloc_Filter_Param() allocates the size of filter's parameter and returns its pointer.
 * mk_Alloc_Filter_Input() allocates the size of filter's input  in the previous step and returns its pointer.
 * mk_Alloc_Filter_Output() allocates the size of filter's output in the previous step and returns its pointer.
 * mk_Cmpt_Filter_Num() computes the number of variables in the previous step.
 */
double* mk_Alloc_Filter_Param(int n_order);
double* mk_Alloc_Filter_Input(int n_order);
double* mk_Alloc_Filter_Output(int n_order);
int mk_Cmpt_Filter_Num(int n_order);

/* mk_Set_Param_1stOrderFilter : Set parmater of 1st-oder Filter 
 *                  a1 + a2 * z^-1
 *          G(z) = ----------------
 *                  a3 + a4 * z^-1
 * 
 * mk_Set_Param_1stOrderFilter_LP : Set parmater of 1st-oder Low-Pass Filter 
 *                                  1
 *          G(s) = gain * -----------------------
 *                         1 + (1/(2*PI*freq)) s
 *
 * mk_Set_Param_1stOrderFilter_HP : Set parmater of 1st-oder High-Pass Filter 
 *                           (1/(2*PI*freq)) s
 *          G(s) = gain * -----------------------
 *                         1 + (1/(2*PI*freq)) s
 *
 * mk_Set_Param_1stOrderFilter_PC : Set parmater of 1st-oder filter for phase compensation 
 *                          1 + (1/(2*PI*freq1)) s
 *          G(s) = gain *  ------------------------
 *                          1 + (1/(2*PI*freq2)) s
 */
void mk_Set_Param_1stOrderFilter(double *param, double a1, double a2, double a3, double a4);
void mk_Set_Param_1stOrderFilter_LP(double *param, double gain, double freq, double dt);
void mk_Set_Param_1stOrderFilter_HP(double *param, double gain, double freq, double dt);
void mk_Set_Param_1stOrderFilter_PC(double *param, double gain, double freq1, double freq2, double dt);


/*****************************************************************************************************
 * MARG
 *****************************************************************************************************/
/* The structure for the attitude estimation using MARG */
typedef struct{
	double m_att[3][3];
	double q_est[4];
	double gyro_est[3];
	double _bias_est[3];
	double _acc0[3];
	double _mag0[3];
} mkAE_MARG;


/* Initialization of the sturcture (mkAE_MARG) 
 * The arguments of this function are as follows:
 * 	- m_att    : Initial attitude matrix
 *	- q_est	   : Initial quaternion
 *	- gyro_est  : Initial angular velocity
 *	- bias_est : Initial bias of the angular velocity
 *	- acc0     : Reference accleration with respect to the sensor frame
 *	- mag0     : Reference magnetism with respect to the sensor frame
 *
 * mkAE_MARG_Initialize() automatically sets m_att, q_est, gyro_est, and bias_est as follows:
 *	- m_att    = the identity matrix
 *	- q_est    = [1 0 0 0]^T
 *	- gyro_est  = [0 0 0]^T
 *	- bias_est = [0 0 0]^T
 */
void mkAE_MARG_Initialize_Core(mkAE_MARG *mkae, double m_att[][3], double *q_est, double *gyro_est, double *bias_est, double *acc0, double *mag0);
void mkAE_MARG_Initialize(mkAE_MARG *mkae, double *acc0, double *mag0);


/* Reset function of the sturcture (mkAE_MARG) 
 * 	- gyro  : Measured angular velocity with respect to the sensor frame
 *	- acc  : Measured accleration with respect to the sensor frame
 *	- mag  : Measured magnetism with respect to the sensor frame
 */
void mkAE_MARG_Reset(mkAE_MARG *mkae, double *gyro, double *acc, double *mag);

/* Explicit complementary filter for MARG (Mahony et al., 2008)
 * The arguments of this function are as follows:
 * 	- gyro  : Measured angular velocity with respect to the sensor frame
 *	- acc  : Measured accleration with respect to the sensor frame
 *	- mag  : Measured magnetism with respect to the sensor frame
 *	- acc0 : Reference accleration with respect to the sensor frame
 *	- mag0 : Reference magnetism with respect to the sensor frame
 *	- wA   : Coefficent for the attitude error between the reference and measured accleration
 *	- wM   : Coefficent for the attitude error between the reference and measured magnetism
 *	- Kp   : Proportional gain of the attitude estimator
 *	- Ki   : Integral gain of the attitude estimator
 *	- dt   : Sampling time
 * 
 * --_Quat and --_AM output the quaternion and attitude matrix respectively.
 */
void mkAE_MARG_MahonyECF_Quat_Core(mkAE_MARG *mkae, double *gyro, double *acc, double *acc0, double *mag, double *mag0, double wA, double wM, double Kp, double Ki, double dt);
void mkAE_MARG_MahonyECF_Quat(mkAE_MARG *mkae, double *gyro, double *acc, double *mag, double wA, double wM, double Kp, double Ki, double dt);
void mkAE_MARG_MahonyECF_AM_Core(mkAE_MARG *mkae, double *gyro, double *acc, double *acc0, double *mag, double *mag0, double wA, double wM, double Kp, double Ki, double dt);
void mkAE_MARG_MahonyECF_AM(mkAE_MARG *mkae, double *gyro, double *acc, double *mag, double wA, double wM, double Kp, double Ki, double dt);




/*****************************************************************************************************/
/* The structure for the attitude estimation by Masuya and Sugihara(2016) */
typedef struct{
	mkAE_MARG mkae;

	/* Filter for the estimate of the acceleration */
	int ord_EstAcc0;
	double *param_EstAcc0Fil[3];
	double *_acc0_est[3];
	double *_acc0_est_f[3];

	/* Filter for the accelerometer's measurement */
	int ord_MesAcc;
	double *param_MesAccFil[3];
	double *_acc_mes[3];
	double *_acc_mes_f[3];

	/* Filter for the estimate of the magnetism */
	int ord_EstMag0;
	double *param_EstMag0Fil[3];
	double *_mag0_est[3];
	double *_mag0_est_f[3];

	/* Filter for the magnetometer's measurement */
	int ord_MesMag;
	double *param_MesMagFil[3];
	double *_mag_mes[3];
	double *_mag_mes_f[3];

	/* Filter for the gyroscope's measurement */
	int ord_Mesgyro;
	double *param_MesgyroFil[3];
	double *_gyro_mes[3];
	double *_gyro_mes_f[3];

} mkAE_MARG_MS2016;


/* Initialization of the sturcture (mkAE_MARG_MS2016) 
 * The arguments of this function are as follows:
 * 	- m_att       : Initial attitude matrix
 *	- q_est	      : Initial quaternion
 *	- gyro_est0    : Initial angular velocity
 *	- bias_est    : Initial bias of the angular velocity
 *	- acc_est0    : Initial accleration
 *	- mag_est0    : Initial magnetism
 *	- acc0        : Reference accleration with respect to the sensor frame
 *	- mag0        : Reference magnetism with respect to the sensor frame
 *	- ord_EstAcc0 : Order of the filter for the estimate of the acceleration
 *	- ord_MesAcc  : Order of the filter for the accelerometer's measurement
 * 	- ord_EstMag0 : Order of the filter for the estimate of the magnetism
 *	- ord_MesMag  : Order of the filter for the magnetometer's measurement
 *	- ord_Mesgyro  : Order of the filter for the gyroscope's measurement
 *
 * mkAE_MARG_MS2016_Initialize() automatically sets m_att, q_est, gyro_est, and bias_est as follows:
 *	- m_att    = the identity matrix
 *	- q_est    = [1 0 0 0]^T
 *	- gyro_est  = gyro0
 *	- bias_est = [0 0 0]^T
 *	- acc_est0 = acc0
 *	- mag_est0 = mag0
 */
void mkAE_MARG_MS2016_Initialize_Core(mkAE_MARG_MS2016 *mkae_ms, double m_att[][3], double *q_est, double *gyro_est0, double *bias_est, double *acc_est0, double *mag_est0, double *acc0, double *mag0,
	int ord_EstAcc0, int ord_MesAcc, int ord_EstMag0, int ord_MesMag, int ord_Mesgyro);
void mkAE_MARG_MS2016_Initialize(mkAE_MARG_MS2016 *mkae_ms, double *gyro0, double *acc0, double *mag0, int ord_EstAcc0, int ord_MesAcc, int ord_EstMag0, int ord_MesMag, int ord_Mesgyro);

/* Reset function of the sturcture (mkAE_MARG_MS2016) */
void mkAE_MARG_MS2016_Reset(mkAE_MARG_MS2016 *mkae_ms, double *gyro, double *acc, double *mag);

/* mkAE_MARG_MS2016_Free() frees the dynamic memory in the sturcture (mkAE_MARG_MS2016) */
void mkAE_MARG_MS2016_Free(mkAE_MARG_MS2016 *mkae_ms);

/* Filter functions 
 * The arguments of this function are as follows:
 * 	- **    : Input of the filter
 *	- **_f	: Output of the filter
 */
void mkAE_MARG_MS2016_Cmpt_EstAcc0Filter(mkAE_MARG_MS2016 *mkae_ms, double *acc0_est, double *acc0_est_f);
void mkAE_MARG_MS2016_Cmpt_MesAccFilter(mkAE_MARG_MS2016 *mkae_ms, double *acc_mes, double *acc_mes_f);
void mkAE_MARG_MS2016_Cmpt_EstMag0Filter(mkAE_MARG_MS2016 *mkae_ms, double *mag0_est, double *mag0_est_f);
void mkAE_MARG_MS2016_Cmpt_MesMagFilter(mkAE_MARG_MS2016 *mkae_ms, double *mag_mes, double *mag_mes_f);
void mkAE_MARG_MS2016_Cmpt_MesgyroFilter(mkAE_MARG_MS2016 *mkae_ms, double *gyro_mes, double *gyro_mes_f);


/* Explicit complementary filter with dynamics Compensation for MARG (Masuya and Sugiahra, 2016) 
 * The arguments of this function are as follows:
 * 	- gyro  : Measured angular velocity with respect to the sensor frame
 *	- acc  : Measured accleration with respect to the sensor frame
 *	- mag  : Measured magnetism with respect to the sensor frame
 *	- acc0 : Reference accleration with respect to the sensor frame
 *	- mag0 : Reference magnetism with respect to the sensor frame
 *	- wA   : Coefficent for the attitude error between the reference and measured accleration
 *	- wM   : Coefficent for the attitude error between the reference and measured magnetism
 *	- Kp   : Proportional gain of the attitude estimator
 *	- Ki   : Integral gain of the attitude estimator
 *	- dt   : Sampling time
 * 
 * --_Quat and --_AM output the quaternion and attitude matrix respectively.
 */
void mkAE_MARG_MS2016_CF_Quat(mkAE_MARG_MS2016 *mkae_ms, double *gyro, double *acc, double *mag, double wA, double wM, double Kp, double Ki, double dt);
void mkAE_MARG_MS2016_CF_AM(mkAE_MARG_MS2016 *mkae_ms, double *gyro, double *acc, double *mag, double wA, double wM, double Kp, double Ki, double dt);


/*****************************************************************************************************
 * IMU
 *****************************************************************************************************/
/* The structure for the attitude estimation using IMU */
typedef struct{
	double m_att[3][3];
	double q_est[4];
	double gyro_est[3];
	double _bias_est[3];
	double _acc0[3];
} mkAE_IMU;


/* Initialization of the sturcture (mkAE_IMU) 
 * The arguments of this function are as follows:
 * 	- m_att    : Initial attitude matrix
 *	- q_est	   : Initial quaternion
 *	- gyro_est  : Initial angular velocity
 *	- bias_est : Initial bias of the angular velocity
 *	- acc0     : Reference accleration with respect to the sensor frame
 *
 * mkAE_IMU_Initialize() automatically sets m_att, q_est, gyro_est, and bias_est as follows:
 *	- m_att    = the identity matrix
 *	- q_est    = [1 0 0 0]^T
 *	- gyro_est  = [0 0 0]^T
 *	- bias_est = [0 0 0]^T
 */
void mkAE_IMU_Initialize_Core(mkAE_IMU *mkae, double m_att[][3], double *q_est, double *gyro_est, double *bias_est, double *acc0);
void mkAE_IMU_Initialize(mkAE_IMU *mkae, double *acc0);

/* Reset function of the sturcture (mkAE_IMU) 
 * 	- gyro  : Measured angular velocity with respect to the sensor frame
 *	- acc  : Measured accleration with respect to the sensor frame
 */
void mkAE_IMU_Reset(mkAE_IMU *mkae, double *gyro, double *acc);

/* Explicit complementary filter for MARG (Mahony et al., 2008)
 * The arguments of this function are as follows:
 * 	- gyro  : Measured angular velocity with respect to the sensor frame
 *	- acc  : Measured accleration with respect to the sensor frame
 *	- acc0 : Reference accleration with respect to the sensor frame
 *	- wA   : Coefficent for the attitude error between the reference and measured accleration
 *	- Kp   : Proportional gain of the attitude estimator
 *	- Ki   : Integral gain of the attitude estimator
 *	- dt   : Sampling time
 * 
 * --_Quat and --_AM output the quaternion and attitude matrix respectively.
 */
void mkAE_IMU_MahonyECF_Quat_Core(mkAE_IMU *mkae, double *gyro, double *acc, double *acc0, double wA, double Kp, double Ki, double dt);
void mkAE_IMU_MahonyECF_Quat(mkAE_IMU *mkae, double *gyro, double *acc, double wA, double Kp, double Ki, double dt);
void mkAE_IMU_MahonyECF_AM_Core(mkAE_IMU *mkae, double *gyro, double *acc, double *acc0, double wA, double Kp, double Ki, double dt);
void mkAE_IMU_MahonyECF_AM(mkAE_IMU *mkae, double *gyro, double *acc, double wA, double Kp, double Ki, double dt);


/*****************************************************************************************************/
/* The structure for the attitude estimation by Masuya and Sugihara(2016) */
typedef struct{
	mkAE_IMU mkae;

	/* Filter for the estimate of the acceleration */
	int ord_EstAcc0;
	double *param_EstAcc0Fil[3];
	double *_acc0_est[3];
	double *_acc0_est_f[3];

	/* Filter for the accelerometer's measurement */
	int ord_MesAcc;
	double *param_MesAccFil[3];
	double *_acc_mes[3];
	double *_acc_mes_f[3];

	/* Filter for the gyroscope's measurement */
	int ord_Mesgyro;
	double *param_MesgyroFil[3];
	double *_gyro_mes[3];
	double *_gyro_mes_f[3];

} mkAE_IMU_MS2016;


/* Initialization of the sturcture (mkAE_IMU_MS2016) 
 * The arguments of this function are as follows:
 * 	- m_att       : Initial attitude matrix
 *	- q_est	      : Initial quaternion
 *	- gyro_est0    : Initial angular velocity
 *	- bias_est    : Initial bias of the angular velocity
 *	- acc_est0    : Initial accleration
 *	- acc0        : Reference accleration with respect to the sensor frame
 *	- ord_EstAcc0 : Order of the filter for the estimate of the acceleration
 *	- ord_MesAcc  : Order of the filter for the accelerometer's measurement
 *	- ord_Mesgyro  : Order of the filter for the gyroscope's measurement
 *
 * mkAE_IMU_MS2016_Initialize() automatically sets m_att, q_est, gyro_est, and bias_est as follows:
 *	- m_att    = the identity matrix
 *	- q_est    = [1 0 0 0]^T
 *	- gyro_est  = gyro0
 *	- bias_est = [0 0 0]^T
 *	- acc_est0 = acc0
 */
void mkAE_IMU_MS2016_Initialize_Core(mkAE_IMU_MS2016 *mkae_ms, double m_att[][3], double *q_est, double *gyro_est0, double *bias_est, double *acc_est0, double *acc0,
	int ord_EstAcc0, int ord_MesAcc, int ord_Mesgyro);
void mkAE_IMU_MS2016_Initialize(mkAE_IMU_MS2016 *mkae_ms, double *gyro0, double *acc0, int ord_EstAcc0, int ord_MesAcc, int ord_Mesgyro);

/* Reset function of the sturcture (mkAE_IMU_MS2016) */
void mkAE_IMU_MS2016_Reset(mkAE_IMU_MS2016 *mkae_ms, double *gyro, double *acc);

/* mkAE_IMU_MS2016_Free() frees the dynamic memory in the sturcture (mkAE_IMU_MS2016) */
void mkAE_IMU_MS2016_Free(mkAE_IMU_MS2016 *mkae_ms);

/* Filter functions 
 * The arguments of this function are as follows:
 * 	- **    : Input of the filter
 *	- **_f	: Output of the filter
 */
void mkAE_IMU_MS2016_Cmpt_EstAcc0Filter(mkAE_IMU_MS2016 *mkae_ms, double *acc0_est, double *acc0_est_f);
void mkAE_IMU_MS2016_Cmpt_MesAccFilter(mkAE_IMU_MS2016 *mkae_ms, double *acc_mes, double *acc_mes_f);
void mkAE_IMU_MS2016_Cmpt_MesgyroFilter(mkAE_IMU_MS2016 *mkae_ms, double *gyro_mes, double *gyro_mes_f);


/* Explicit complementary filter with dynamics Compensation for MARG (Masuya and Sugiahra, 2016) 
 * The arguments of this function are as follows:
 * 	- gyro  : Measured angular velocity with respect to the sensor frame
 *	- acc  : Measured accleration with respect to the sensor frame
 *	- acc0 : Reference accleration with respect to the sensor frame
 *	- wA   : Coefficent for the attitude error between the reference and measured accleration
 *	- Kp   : Proportional gain of the attitude estimator
 *	- Ki   : Integral gain of the attitude estimator
 *	- dt   : Sampling time
 * 
 * --_Quat and --_AM output the quaternion and attitude matrix respectively.
 */
void mkAE_IMU_MS2016_CF_Quat(mkAE_IMU_MS2016 *mkae_ms, double *gyro, double *acc, double wA, double Kp, double Ki, double dt);
void mkAE_IMU_MS2016_CF_AM(mkAE_IMU_MS2016 *mkae_ms, double *gyro, double *acc, double wA, double Kp, double Ki, double dt);



