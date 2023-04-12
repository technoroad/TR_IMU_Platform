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

#include "mkAE_functions.h"

/*****************************************************************************************************/
/* Computation of the 3-D matrix multiplication, m3 = m1.m2 */
void __attribute__((optimize("O0"))) mk_Cmpt_AM_AM(double m1[][3], double m2[][3], double m3[][3]){
	int i, j, k;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			m3[i][j] = 0.0;
			for(k=0;k<3;k++) m3[i][j] = m3[i][j]+m1[i][k]*m2[k][j];
		}
	}		
}
/* Computation of the product of the 3-D matrix and 3-D vector, v2 = m1.v1 */
void __attribute__((optimize("O0"))) mk_Cmpt_AM_Vec(double m1[][3], double *v1, double *v2){
	int i, j;
	for(i=0;i<3;i++){
		v2[i] = 0.0;
		for(j=0;j<3;j++) v2[i] = v2[i]+m1[i][j]*v1[j];
	}		
}
/* Computation of the product of the 3-D matrix (transposed) and 3-D vector, v2 = m1^T.v1 */
void __attribute__((optimize("O0"))) mk_Cmpt_AMT_Vec(double m1[][3], double *v1, double *v2){
	int i, j;
	for(i=0;i<3;i++){
		v2[i] = 0.0;
		for(j=0;j<3;j++) v2[i] = v2[i]+m1[j][i]*v1[j];
	}		
}
/* Computation of the outer product of the 3-D vector, v3 = v1 times v2 */
void __attribute__((optimize("O0"))) mk_Cmpt_OuterProduct(double *v1, double *v2, double *v3){
	v3[0] = v1[1] * v2[2] - v1[2] * v2[1];
	v3[1] = v1[2] * v2[0] - v1[0] * v2[2];
	v3[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

/* Computation of Z-Y-X Euler angle from the attitude matrix */
void __attribute__((optimize("O0"))) mk_Cmpt_ZYXangle_AM(double m_att[][3], double *ang){
	ang[0] = atan2( m_att[1][0], m_att[0][0] );
	ang[1] = atan2( -m_att[2][0], m_att[0][0]*cos(ang[0])+m_att[1][0]*sin(ang[0]) );
	ang[2] = atan2( m_att[0][2]*sin(ang[0])-m_att[1][2]*cos(ang[0]), -m_att[0][1]*sin(ang[0])+m_att[1][1]*cos(ang[0]));
}
/* Computation of the attitude matrix from Z-Y-X Euler angle */
void __attribute__((optimize("O0"))) mk_Cmpt_AM_ZYXangle(double *ang, double m_att[][3]){
	m_att[0][0] = cos(ang[0])*cos(ang[1]);
	m_att[1][0] = sin(ang[0])*cos(ang[1]);
	m_att[2][0] = -sin(ang[1]);

	m_att[0][1] = cos(ang[0])*sin(ang[1])*sin(ang[2])-sin(ang[0])*cos(ang[2]);
	m_att[1][1] = sin(ang[0])*sin(ang[1])*sin(ang[2])+cos(ang[0])*cos(ang[2]);
	m_att[2][1] = cos(ang[1])*sin(ang[2]);

	m_att[0][2] = cos(ang[0])*sin(ang[1])*cos(ang[2])+sin(ang[0])*sin(ang[2]);
	m_att[1][2] = sin(ang[0])*sin(ang[1])*cos(ang[2])-cos(ang[0])*sin(ang[2]);
	m_att[2][2] = cos(ang[1])*cos(ang[2]);
}
/* Computation of Z-Y-X Euler angle from the quaternion */
void __attribute__((optimize("O0"))) mk_Cmpt_ZYXangle_Quat(double *q, double *ang){
	ang[0] = atan2( 2.0*(q[0]*q[3]+q[1]*q[2]), q[0]*q[0]+q[1]*q[1]-q[2]*q[2]-q[3]*q[3] );
	ang[1] = asin( 2.0*(q[0]*q[2]-q[1]*q[3]) );
	ang[2] = atan2( 2.0*(q[0]*q[1]+q[2]*q[3]), q[0]*q[0]-q[1]*q[1]-q[2]*q[2]+q[3]*q[3] );
}
/* Computation of the quaternion from Z-Y-X Euler angle */
void __attribute__((optimize("O0"))) mk_Cmpt_Quat_ZYXangle(double *ang, double *q){
	q[0] = cos(ang[0]/2.0)*cos(ang[1]/2.0)*cos(ang[2]/2.0) + sin(ang[0]/2.0)*sin(ang[1]/2.0)*sin(ang[2]/2.0);
	q[1] = sin(ang[0]/2.0)*cos(ang[1]/2.0)*cos(ang[2]/2.0) - cos(ang[0]/2.0)*sin(ang[1]/2.0)*sin(ang[2]/2.0);
	q[2] = cos(ang[0]/2.0)*sin(ang[1]/2.0)*cos(ang[2]/2.0) + sin(ang[0]/2.0)*cos(ang[1]/2.0)*sin(ang[2]/2.0);
	q[3] = cos(ang[0]/2.0)*cos(ang[1]/2.0)*sin(ang[2]/2.0) - sin(ang[0]/2.0)*sin(ang[1]/2.0)*cos(ang[2]/2.0);
}
/* Computation of the attitude matrix from the quaternion */
void __attribute__((optimize("O0"))) mk_Cmpt_AM_Quat(double *q, double m_att[][3]){
	m_att[0][0] = q[0]*q[0]+q[1]*q[1]-q[2]*q[2]-q[3]*q[3];
	m_att[1][0] = 2.0*(q[1]*q[2]+q[0]*q[3]);
	m_att[2][0] = 2.0*(q[1]*q[3]-q[0]*q[2]);

	m_att[0][1] = 2.0*(q[1]*q[2]-q[0]*q[3]);
	m_att[1][1] = q[0]*q[0]-q[1]*q[1]+q[2]*q[2]-q[3]*q[3];
	m_att[2][1] = 2.0*(q[0]*q[1]+q[2]*q[3]);

	m_att[0][2] = 2.0*(q[1]*q[3]+q[0]*q[2]);
	m_att[1][2] = 2.0*(q[2]*q[3]-q[0]*q[1]);
	m_att[2][2] = q[0]*q[0]-q[1]*q[1]-q[2]*q[2]+q[3]*q[3];
}
/* Computation of the quaternion from the attitude matrix */
void __attribute__((optimize("O0"))) mk_Cmpt_Quat_AM(double m_att[][3], double *q){
	if(m_att[0][0]+m_att[1][1]+m_att[2][2]+1.0 > pow(10.0, -10.0)){
		q[0] = sqrt(m_att[0][0]+m_att[1][1]+m_att[2][2]+1.0)/2.0;
		q[1] = -(m_att[1][2]-m_att[2][1])/(4.0*q[0]);
		q[2] = -(m_att[2][0]-m_att[0][2])/(4.0*q[0]);
		q[3] = -(m_att[0][1]-m_att[1][0])/(4.0*q[0]);
	}
	else if(m_att[0][0]-m_att[1][1]-m_att[2][2]+1.0 > pow(10.0, -10.0)){
		q[1] = sqrt(m_att[0][0]-m_att[1][1]-m_att[2][2]+1.0)/2.0;
		q[0] = -(m_att[1][2]-m_att[2][1])/(4.0*q[1]);
		q[2] = -(m_att[0][1]+m_att[1][0])/(4.0*q[1]);
		q[3] = -(m_att[2][0]+m_att[0][2])/(4.0*q[1]);
	}
	else if(-m_att[0][0]+m_att[1][1]-m_att[2][2]+1.0 > pow(10.0, -10.0)){
		q[2] = sqrt(-m_att[0][0]+m_att[1][1]-m_att[2][2]+1.0)/2.0;
		q[0] = -(m_att[2][0]-m_att[0][2])/(4.0*q[2]);
		q[1] = -(m_att[0][1]+m_att[1][0])/(4.0*q[2]);
		q[3] = -(m_att[1][2]+m_att[2][1])/(4.0*q[2]);
	}
	else{
		q[3] = sqrt(-m_att[0][0]-m_att[1][1]+m_att[2][2]+1.0)/2.0;
		q[0] = -(m_att[0][1]-m_att[1][0])/(4.0*q[3]);
		q[1] = -(m_att[2][0]+m_att[0][2])/(4.0*q[3]);
		q[2] = -(m_att[1][2]+m_att[2][1])/(4.0*q[3]);
	}
}

/* Computation of the matrix based on the Rodrigues's rotation formula */
void __attribute__((optimize("O0"))) mk_Cmpt_AM_Rodrigues(double m[][3], double *v){
	int i, j;
	double theta, v1[3], s_t, c_t;
	theta = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	for(i=0;i<3;i++){
		for(j=0;j<3;j++) m[i][j] = 0.0;
	}
	if(theta<pow(10.0, -16.0)){
		for(i=0;i<3;i++) m[i][i] = 1.0;
	}
	else{
		s_t = sin(theta);
		c_t = cos(theta);
		for(i=0;i<3;i++) v1[i] = v[i]/theta;
		for(i=0;i<3;i++) m[i][i] = c_t + v1[i]*v1[i]*(1.0-c_t);
		m[0][1] = v1[0]*v1[1]*(1.0-c_t) - v1[2]*s_t;
		m[0][2] = v1[0]*v1[2]*(1.0-c_t) + v1[1]*s_t;
		m[1][0] = v1[0]*v1[1]*(1.0-c_t) + v1[2]*s_t;
		m[1][2] = v1[1]*v1[2]*(1.0-c_t) - v1[0]*s_t;
		m[2][0] = v1[2]*v1[0]*(1.0-c_t) - v1[1]*s_t;
		m[2][1] = v1[2]*v1[1]*(1.0-c_t) + v1[0]*s_t;
	}
}
/* Computation of the vector rotated by the quaternion, v2 = R(q1).v1 */
void __attribute__((optimize("O0"))) mk_Cmpt_Rotate_Quat_Vec(double *q1, double *v1, double *v2){
	v2[0] = (q1[0]*q1[0]+q1[1]*q1[1]-q1[2]*q1[2]-q1[3]*q1[3])*v1[0] + 2*(q1[1]*q1[2]+q1[0]*q1[3])*v1[1] + 2*(q1[1]*q1[3]-q1[0]*q1[2])*v1[2];
	v2[1] = 2*(q1[1]*q1[2]-q1[0]*q1[3])*v1[0] + (q1[0]*q1[0]-q1[1]*q1[1]+q1[2]*q1[2]-q1[3]*q1[3])*v1[1] + 2*(q1[2]*q1[3]+q1[0]*q1[1])*v1[2];
	v2[2] = 2*(q1[1]*q1[3]+q1[0]*q1[2])*v1[0] + 2*(q1[2]*q1[3]-q1[0]*q1[1])*v1[1] + (q1[0]*q1[0]-q1[1]*q1[1]-q1[2]*q1[2]+q1[3]*q1[3])*v1[2];
}
/* Computation of the quaternion derivative, q2 = (1/2)*q1*gyro */
void __attribute__((optimize("O0"))) mk_Cmpt_Quat_Derivative(double *q1, double *gyro, double *q2){
	q2[0] = -0.5*(                gyro[0]*q1[1] + gyro[1]*q1[2] + gyro[2]*q1[3] );
	q2[1] = 0.5* (gyro[0]*q1[0]                 + gyro[2]*q1[2] - gyro[1]*q1[3] );
	q2[2] = 0.5* (gyro[1]*q1[0] - gyro[2]*q1[1]                 + gyro[0]*q1[3] );
	q2[3] = 0.5* (gyro[2]*q1[0] + gyro[1]*q1[1] - gyro[0]*q1[2]                 );
}


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
void __attribute__((optimize("O0"))) mk_Cmpt_1stOrderFilter(double *a_mes, double *a_mes_m, double *a_est_m, double *param, double *a_est){
	*a_est = param[0]*(*a_mes) + param[1]*(*a_mes_m) - param[3]*(*a_est_m);
	*a_est = (*a_est)/(param[2]);

	(*a_mes_m) = (*a_mes);
	(*a_est_m) = (*a_est);
}
void __attribute__((optimize("O0"))) mk_Cmpt_2ndOrderFilter(double *a_mes, double *a_mes_m, double *a_est_m, double *param, double *a_est){
	*a_est = param[0]*(*a_mes) + param[1]*(a_mes_m[0]) + param[2]*(a_mes_m[1]) - param[4]*(a_est_m[0]) - param[5]*(a_est_m[1]);
	*a_est = (*a_est)/(param[3]);

	a_mes_m[1] = a_mes_m[0];
	a_mes_m[0] = (*a_mes);
	a_est_m[1] = a_est_m[0];
	a_est_m[0] = (*a_est);
}
void __attribute__((optimize("O0"))) mk_Cmpt_012_OrderFilter(int n_order, double *a_mes, double *a_mes_m, double *a_est_m, double *param, double *a_est){
	switch(n_order){
		case 0 : *a_est = (param[0])*(*a_mes);
			 break;
		case 1 : mk_Cmpt_1stOrderFilter(a_mes, &a_mes_m[0], &a_est_m[0], param, a_est);
			 break;
		case 2 : mk_Cmpt_2ndOrderFilter(a_mes, a_mes_m, a_est_m, param, a_est);
			 break;
		default: break;
	}
}
/* Allocate the filter
 * The argument of those functions is as follows:
 *	- n_order : Order of the filter 
 *
 * mk_Alloc_Filter_Param() allocates the size of filter's parameter and returns its pointer.
 * mk_Alloc_Filter_Input() allocates the size of filter's input  in the previous step and returns its pointer.
 * mk_Alloc_Filter_Output() allocates the size of filter's output in the previous step and returns its pointer.
 * mk_Cmpt_Filter_Num() computes the number of variables in the previous step.
 */
double* __attribute__((optimize("O0"))) mk_Alloc_Filter_Param(int n_order){
	double *param;
	switch(n_order){
		case 0: param = malloc(sizeof(double) * 1);
			break;
		case 1: param = malloc(sizeof(double) * 4);
			break;
		case 2: param = malloc(sizeof(double) * 6);
			break;
		default: param = malloc(sizeof(double) * 1);
			break;
	}
	return(param);
}
double* __attribute__((optimize("O0"))) mk_Alloc_Filter_Input(int n_order){
	double *a_mes;
	switch(n_order){
		case 0: a_mes = malloc(sizeof(double) * 1);
			break;
		case 1: a_mes = malloc(sizeof(double) * 1);
			break;
		case 2: a_mes = malloc(sizeof(double) * 2);
			break;
		default: a_mes = malloc(sizeof(double) * 1);
			break;
	}
	return(a_mes);
}
double* __attribute__((optimize("O0"))) mk_Alloc_Filter_Output(int n_order){
	double *a_est;
	switch(n_order){
		case 0: a_est = malloc(sizeof(double) * 1);
			break;
		case 1: a_est = malloc(sizeof(double) * 1);
			break;
		case 2: a_est = malloc(sizeof(double) * 2);
			break;
		default: a_est = malloc(sizeof(double) * 1);
			break;
	}
	return(a_est);
}
int __attribute__((optimize("O0"))) mk_Cmpt_Filter_Num(int n_order){
	int num_fil = 0;
	switch(n_order){
		case 0: num_fil = 1;
			break;
		case 1: num_fil = 1;
			break;
		case 2: num_fil = 2;
			break;
		default: break;
	}
	return(num_fil);
}
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
void __attribute__((optimize("O0"))) mk_Set_Param_1stOrderFilter(double *param, double a1, double a2, double a3, double a4){
	param[0] = a1;
	param[1] = a2;
	param[2] = a3;
	param[3] = a4;
}
void __attribute__((optimize("O0"))) mk_Set_Param_1stOrderFilter_LP(double *param, double gain, double freq, double dt){
	mk_Set_Param_1stOrderFilter(param, gain*dt, gain*dt, dt+1.0/(M_PI*freq), dt-1.0/(M_PI*freq));
}
void __attribute__((optimize("O0"))) mk_Set_Param_1stOrderFilter_HP(double *param, double gain, double freq, double dt){
	mk_Set_Param_1stOrderFilter(param, gain/(M_PI*freq), -gain/(M_PI*freq), dt+1.0/(M_PI*freq), dt-1.0/(M_PI*freq));
}
void __attribute__((optimize("O0"))) mk_Set_Param_1stOrderFilter_PC(double *param, double gain, double freq1, double freq2, double dt){
	mk_Set_Param_1stOrderFilter(param, gain*(dt + 1.0/(M_PI*freq1)), gain*(dt - 1.0/(M_PI*freq1)), dt+1.0/(M_PI*freq2), dt-1.0/(M_PI*freq2) );
}


/*****************************************************************************************************
 * MARG
 *****************************************************************************************************/
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
void __attribute__((optimize("O0"))) mkAE_MARG_Initialize_Core(mkAE_MARG *mkae, double m_att[][3], double *q_est, double *gyro_est, double *bias_est, double *acc0, double *mag0){
	int i, j;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++) mkae->m_att[i][j] = m_att[i][j];
		mkae->_bias_est[i] = bias_est[i];
		mkae->_mag0[i] = mag0[i];
		mkae->_acc0[i] = acc0[i];
		mkae->gyro_est[i] = gyro_est[i];
	}
	for(i=0;i<4;i++) mkae->q_est[i] = q_est[i];
}
void __attribute__((optimize("O0"))) mkAE_MARG_Initialize(mkAE_MARG *mkae, double *acc0, double *mag0){
	int i, j;
	double bias_est[3], m_att[3][3], gyro_est[3], q_est[4];

	for(i=0;i<3;i++){
		for(j=0;j<3;j++) m_att[i][j] = 0.0;
		m_att[i][i] = 1.0;
		bias_est[i] = 0.0;
		gyro_est[i] = 0.0;
		q_est[i+1] = 0.0;
	}
	q_est[0] = 1.0;
	mkAE_MARG_Initialize_Core(mkae, m_att, q_est, gyro_est, bias_est, acc0, mag0);
}

/* Reset function of the sturcture (mkAE_MARG) 
 * 	- gyro  : Measured angular velocity with respect to the sensor frame
 *	- acc  : Measured accleration with respect to the sensor frame
 *	- mag  : Measured magnetism with respect to the sensor frame
 */
void __attribute__((optimize("O0"))) mkAE_MARG_Reset(mkAE_MARG *mkae, double *gyro, double *acc, double *mag){
	int i, j;
	double bias_est[3], m_att[3][3], q_est[4];

	for(i=0;i<3;i++){
		for(j=0;j<3;j++) m_att[i][j] = 0.0;
		m_att[i][i] = 1.0;
		bias_est[i] = 0.0;
		q_est[i+1] = 0.0;
	}
	q_est[0] = 1.0;
	mkAE_MARG_Initialize_Core(mkae, m_att, q_est, gyro, bias_est, acc, mag);
}

double my_wA[3] ={1,1,0};

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
/* Outputs the quaternion */
void __attribute__((optimize("O0"))) mkAE_MARG_MahonyECF_Quat_Core(mkAE_MARG *mkae, double *gyro, double *acc, double *acc0, double *mag, double *mag0, double wA, double wM, double Kp, double Ki, double dt){
	int i;
	double norm=0.0;
	double q1[4];
	double gyro_acc[3], gyro_mag[3], v1[3];

	/* Error vector calculated by Accelerometer and Magnetometer outputs */
	/* Accel */
	mk_Cmpt_OuterProduct(acc, acc0, gyro_acc);
	/* Magnet */
	mk_Cmpt_OuterProduct(mag, mag0, gyro_mag);
	for(i=0;i<3;i++){
//		v1[i] = wA*gyro_acc[i] + wM*gyro_mag[i];
		v1[i] = my_wA[i]*gyro_acc[i] + wM*gyro_mag[i];
		mkae->_bias_est[i] = mkae->_bias_est[i]-Ki*dt*v1[i];
	}

	/* Angular velcoity compensation */
	for(i=0;i<3;i++){
		mkae->gyro_est[i] = gyro[i] - (mkae->_bias_est[i]) + Kp*v1[i];
	}

	/* Update the quaternion */
	mk_Cmpt_Quat_Derivative(mkae->q_est, mkae->gyro_est, q1);
	for(i=0;i<4;i++){
		mkae->q_est[i] = mkae->q_est[i] + q1[i]*dt;
		norm = norm + (mkae->q_est[i])*(mkae->q_est[i]);
	}
	for(i=0;i<4;i++) mkae->q_est[i] = (mkae->q_est[i])/sqrt(norm);
	mk_Cmpt_AM_Quat(mkae->q_est, mkae->m_att);
}
void __attribute__((optimize("O0"))) mkAE_MARG_MahonyECF_Quat(mkAE_MARG *mkae, double *gyro, double *acc, double *mag, double wA, double wM, double Kp, double Ki, double dt){
	double acc0[3], mag0[3];
	mk_Cmpt_Rotate_Quat_Vec(mkae->q_est, mkae->_acc0, acc0);
	mk_Cmpt_Rotate_Quat_Vec(mkae->q_est, mkae->_mag0, mag0);
	mkAE_MARG_MahonyECF_Quat_Core(mkae, gyro, acc, acc0, mag, mag0, wA, wM, Kp, Ki, dt);
}
/* Outputs the attitude matrix */
void __attribute__((optimize("O0"))) mkAE_MARG_MahonyECF_AM_Core(mkAE_MARG *mkae, double *gyro, double *acc, double *acc0, double *mag, double *mag0, double wA, double wM, double Kp, double Ki, double dt){
	int i, j;
	double m1[3][3], m2[3][3];
	double gyro_acc[3], gyro_mag[3], v1[3];

	/* Error vector calculated by Accelerometer and Magnetometer outputs */
	/* Accel */
	mk_Cmpt_OuterProduct(acc, acc0, gyro_acc);
	/* Magnet */
	mk_Cmpt_OuterProduct(mag, mag0, gyro_mag);
	for(i=0;i<3;i++){
		v1[i] = wA * gyro_acc[i] + wM * gyro_mag[i];
		mkae->_bias_est[i] = mkae->_bias_est[i]-Ki*dt*v1[i];
	}	

	/* Angular velcoity compensation */
	for(i=0;i<3;i++){
		mkae->gyro_est[i] = gyro[i] - (mkae->_bias_est[i]) + Kp*v1[i];
		v1[i] = dt*(mkae->gyro_est[i]);
	} 
	
	/* Update the attitude matrix */
	mk_Cmpt_AM_Rodrigues(m1, v1);
	mk_Cmpt_AM_AM(mkae->m_att, m1, m2);
	for(i=0;i<3;i++){
		for(j=0;j<3;j++) mkae->m_att[i][j] = m2[i][j];
	}
	mk_Cmpt_Quat_AM(mkae->m_att, mkae->q_est);
}
void __attribute__((optimize("O0"))) mkAE_MARG_MahonyECF_AM(mkAE_MARG *mkae, double *gyro, double *acc, double *mag, double wA, double wM, double Kp, double Ki, double dt){
	double acc0[3], mag0[3];
	mk_Cmpt_AMT_Vec(mkae->m_att, mkae->_acc0, acc0);
	mk_Cmpt_AMT_Vec(mkae->m_att, mkae->_mag0, mag0);
	mkAE_MARG_MahonyECF_AM_Core(mkae, gyro, acc, acc0, mag, mag0, wA, wM, Kp, Ki, dt);
}




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
void __attribute__((optimize("O0"))) mkAE_MARG_MS2016_Initialize_Core(mkAE_MARG_MS2016 *mkae_ms, double m_att[][3], double *q_est, double *gyro_est0, double *bias_est, double *acc_est0, double *mag_est0, double *acc0, double *mag0, int ord_EstAcc0, int ord_MesAcc, int ord_EstMag0, int ord_MesMag, int ord_Mesgyro){
	int i, j;
	
	mkAE_MARG_Initialize_Core(&(mkae_ms->mkae), m_att, q_est, gyro_est0, bias_est, acc0, mag0);

	mkae_ms->ord_EstAcc0 = ord_EstAcc0;
	mkae_ms->ord_MesAcc = ord_MesAcc;
	mkae_ms->ord_EstMag0 = ord_EstMag0;
	mkae_ms->ord_MesMag = ord_MesMag;
	mkae_ms->ord_Mesgyro = ord_Mesgyro;
	
	for(i=0;i<3;i++) mkae_ms->param_EstAcc0Fil[i] = mk_Alloc_Filter_Param(ord_EstAcc0);
	for(i=0;i<3;i++) mkae_ms->_acc0_est[i] = mk_Alloc_Filter_Input(ord_EstAcc0);
	for(i=0;i<3;i++) mkae_ms->_acc0_est_f[i] = mk_Alloc_Filter_Output(ord_EstAcc0);
	
	for(i=0;i<3;i++) mkae_ms->param_MesAccFil[i] = mk_Alloc_Filter_Param(ord_MesAcc);
	for(i=0;i<3;i++) mkae_ms->_acc_mes[i] = mk_Alloc_Filter_Input(ord_MesAcc);
	for(i=0;i<3;i++) mkae_ms->_acc_mes_f[i] = mk_Alloc_Filter_Output(ord_MesAcc);
		
	for(i=0;i<3;i++) mkae_ms->param_EstMag0Fil[i] = mk_Alloc_Filter_Param(ord_EstMag0);
	for(i=0;i<3;i++) mkae_ms->_mag0_est[i] = mk_Alloc_Filter_Input(ord_EstMag0);
	for(i=0;i<3;i++) mkae_ms->_mag0_est_f[i] = mk_Alloc_Filter_Output(ord_EstMag0);
		
	for(i=0;i<3;i++) mkae_ms->param_MesMagFil[i] = mk_Alloc_Filter_Param(ord_MesMag);
	for(i=0;i<3;i++) mkae_ms->_mag_mes[i] = mk_Alloc_Filter_Input(ord_MesMag);
	for(i=0;i<3;i++) mkae_ms->_mag_mes_f[i] = mk_Alloc_Filter_Output(ord_MesMag);

	for(i=0;i<3;i++) mkae_ms->param_MesgyroFil[i] = mk_Alloc_Filter_Param(ord_Mesgyro);
	for(i=0;i<3;i++) mkae_ms->_gyro_mes[i] = mk_Alloc_Filter_Input(ord_Mesgyro);
	for(i=0;i<3;i++) mkae_ms->_gyro_mes_f[i] = mk_Alloc_Filter_Output(ord_Mesgyro);

	for(i=0;i<3;i++){
		for(j=0;j<mk_Cmpt_Filter_Num(ord_EstAcc0);j++){
			mkae_ms->_acc0_est[i][j] = acc_est0[i];
			mkae_ms->_acc0_est_f[i][j] = acc_est0[i];
		}
		for(j=0;j<mk_Cmpt_Filter_Num(ord_MesAcc);j++){
			mkae_ms->_acc_mes[i][j] = acc_est0[i];
			mkae_ms->_acc_mes_f[i][j] = acc_est0[i];
		}
		for(j=0;j<mk_Cmpt_Filter_Num(ord_EstMag0);j++){
			mkae_ms->_mag0_est[i][j] = mag_est0[i];
			mkae_ms->_mag0_est_f[i][j] = mag_est0[i];
		}
		for(j=0;j<mk_Cmpt_Filter_Num(ord_MesMag);j++){
			mkae_ms->_mag_mes[i][j] = mag_est0[i];
			mkae_ms->_mag_mes_f[i][j] = mag_est0[i];
		}
		for(j=0;j<mk_Cmpt_Filter_Num(ord_Mesgyro);j++){
			mkae_ms->_gyro_mes[i][j] = gyro_est0[i];
			mkae_ms->_gyro_mes_f[i][j] = gyro_est0[i];
		}
	}
}
void __attribute__((optimize("O0"))) mkAE_MARG_MS2016_Initialize(mkAE_MARG_MS2016 *mkae_ms, double *gyro0, double *acc0, double *mag0, int ord_EstAcc0, int ord_MesAcc, int ord_EstMag0, int ord_MesMag, int ord_Mesgyro){
	int i, j;
	double bias_est[3], m_att[3][3], q_est[4];
	
	for(i=0;i<3;i++){
		for(j=0;j<3;j++) m_att[i][j] = 0.0;
		m_att[i][i] = 1.0;
		bias_est[i] = 0.0;
		q_est[i+1] = 0.0;
	}
	q_est[0] = 1.0;
	mkAE_MARG_MS2016_Initialize_Core(mkae_ms, m_att, q_est, gyro0, bias_est, acc0, mag0, acc0, mag0, ord_EstAcc0, ord_MesAcc, ord_EstMag0, ord_MesMag, ord_Mesgyro);
}

/* Reset function of the sturcture (mkAE_MARG_MS2016) */
void __attribute__((optimize("O0"))) mkAE_MARG_MS2016_Reset(mkAE_MARG_MS2016 *mkae_ms, double *gyro, double *acc, double *mag){
	int i, j;
	mkAE_MARG_Reset(&(mkae_ms->mkae), gyro, acc, mag);
	for(i=0;i<3;i++){
		for(j=0;j<mk_Cmpt_Filter_Num(mkae_ms->ord_EstAcc0);j++){
			mkae_ms->_acc0_est[i][j] = acc[i];
			mkae_ms->_acc0_est_f[i][j] = acc[i];
		}
		for(j=0;j<mk_Cmpt_Filter_Num(mkae_ms->ord_MesAcc);j++){
			mkae_ms->_acc_mes[i][j] = acc[i];
			mkae_ms->_acc_mes_f[i][j] = acc[i];
		}
		for(j=0;j<mk_Cmpt_Filter_Num(mkae_ms->ord_EstMag0);j++){
			mkae_ms->_mag0_est[i][j] = mag[i];
			mkae_ms->_mag0_est_f[i][j] = mag[i];
		}
		for(j=0;j<mk_Cmpt_Filter_Num(mkae_ms->ord_MesMag);j++){
			mkae_ms->_mag_mes[i][j] = mag[i];
			mkae_ms->_mag_mes_f[i][j] = mag[i];
		}
		for(j=0;j<mk_Cmpt_Filter_Num(mkae_ms->ord_Mesgyro);j++){
			mkae_ms->_gyro_mes[i][j] = gyro[i];
			mkae_ms->_gyro_mes_f[i][j] = gyro[i];
		}
	}
}
	

/* mkAE_MARG_MS2016_Free() frees the dynamic memory in the sturcture (mkAE_MARG_MS2016) */
void __attribute__((optimize("O0"))) mkAE_MARG_MS2016_Free(mkAE_MARG_MS2016 *mkae_ms){
	int i;
	for(i=0;i<3;i++){
		free(mkae_ms->param_EstAcc0Fil[i]);
		mkae_ms->param_EstAcc0Fil[i] = NULL;
		free(mkae_ms->_acc0_est[i]);
		mkae_ms->_acc0_est[i] = NULL;
		free(mkae_ms->_acc0_est_f[i]);
		mkae_ms->_acc0_est_f[i] = NULL;
	
		free(mkae_ms->param_MesAccFil[i]);
		mkae_ms->param_MesAccFil[i] = NULL;
		free(mkae_ms->_acc_mes[i]);
		mkae_ms->_acc_mes[i] = NULL;
		free(mkae_ms->_acc_mes_f[i]);
		mkae_ms->_acc_mes_f[i] = NULL;
	
		free(mkae_ms->param_EstMag0Fil[i]);
		mkae_ms->param_EstMag0Fil[i] = NULL;
		free(mkae_ms->_mag0_est[i]);
		mkae_ms->_mag0_est[i] = NULL;
		free(mkae_ms->_mag0_est_f[i]);
		mkae_ms->_mag0_est_f[i] = NULL;

		free(mkae_ms->param_MesMagFil[i]);
		mkae_ms->param_MesMagFil[i] = NULL;
		free(mkae_ms->_mag_mes[i]);
		mkae_ms->_mag_mes[i] = NULL;
		free(mkae_ms->_mag_mes_f[i]);
		mkae_ms->_mag_mes_f[i] = NULL;

		free(mkae_ms->param_MesgyroFil[i]);
		mkae_ms->param_MesgyroFil[i] = NULL;
		free(mkae_ms->_gyro_mes[i]);
		mkae_ms->_gyro_mes[i] = NULL;
		free(mkae_ms->_gyro_mes_f[i]);
		mkae_ms->_gyro_mes_f[i] = NULL;
	}
}


/* Filter functions 
 * The arguments of this function are as follows:
 * 	- **    : Input of the filter
 *	- **_f	: Output of the filter
 */
void __attribute__((optimize("O0"))) mkAE_MARG_MS2016_Cmpt_EstAcc0Filter(mkAE_MARG_MS2016 *mkae_ms, double *acc0_est, double *acc0_est_f){
	int i;
	for(i=0;i<3;i++) mk_Cmpt_012_OrderFilter(mkae_ms->ord_EstAcc0, &acc0_est[i], mkae_ms->_acc0_est[i], mkae_ms->_acc0_est_f[i], mkae_ms->param_EstAcc0Fil[i], &acc0_est_f[i]);
}
void __attribute__((optimize("O0"))) mkAE_MARG_MS2016_Cmpt_MesAccFilter(mkAE_MARG_MS2016 *mkae_ms, double *acc_mes, double *acc_mes_f){
	int i;
	for(i=0;i<3;i++) mk_Cmpt_012_OrderFilter(mkae_ms->ord_MesAcc, &acc_mes[i], mkae_ms->_acc_mes[i], mkae_ms->_acc_mes_f[i], mkae_ms->param_MesAccFil[i], &acc_mes_f[i]);
}
void __attribute__((optimize("O0"))) mkAE_MARG_MS2016_Cmpt_EstMag0Filter(mkAE_MARG_MS2016 *mkae_ms, double *mag0_est, double *mag0_est_f){
	int i;
	for(i=0;i<3;i++) mk_Cmpt_012_OrderFilter(mkae_ms->ord_EstMag0, &mag0_est[i], mkae_ms->_mag0_est[i], mkae_ms->_mag0_est_f[i], mkae_ms->param_EstMag0Fil[i], &mag0_est_f[i]);
}
void __attribute__((optimize("O0"))) mkAE_MARG_MS2016_Cmpt_MesMagFilter(mkAE_MARG_MS2016 *mkae_ms, double *mag_mes, double *mag_mes_f){
	int i;
	for(i=0;i<3;i++) mk_Cmpt_012_OrderFilter(mkae_ms->ord_MesMag, &mag_mes[i], mkae_ms->_mag_mes[i], mkae_ms->_mag_mes_f[i], mkae_ms->param_MesMagFil[i], &mag_mes_f[i]);
}
void __attribute__((optimize("O0"))) mkAE_MARG_MS2016_Cmpt_MesgyroFilter(mkAE_MARG_MS2016 *mkae_ms, double *gyro_mes, double *gyro_mes_f){
	int i;
	for(i=0;i<3;i++) mk_Cmpt_012_OrderFilter(mkae_ms->ord_Mesgyro, &gyro_mes[i], mkae_ms->_gyro_mes[i], mkae_ms->_gyro_mes_f[i], mkae_ms->param_MesgyroFil[i], &gyro_mes_f[i]);
}



/* Explicit complementary filter with dynamics Compensation for MARG (Masuya&Sugiahra, 2016) 
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
/* Outputs the quaternion */
void __attribute__((optimize("O0"))) mkAE_MARG_MS2016_CF_Quat(mkAE_MARG_MS2016 *mkae_ms, double *gyro, double *acc, double *mag, double wA, double wM, double Kp, double Ki, double dt){
	double acc0_est[3], mag0_est[3], gyro_f[3], acc_f[3], mag_f[3], v1[3];

	mk_Cmpt_Rotate_Quat_Vec(mkae_ms->mkae.q_est, mkae_ms->mkae._acc0, v1);
	mkAE_MARG_MS2016_Cmpt_EstAcc0Filter(mkae_ms, v1, acc0_est);
	mk_Cmpt_Rotate_Quat_Vec(mkae_ms->mkae.q_est, mkae_ms->mkae._mag0, v1);	
	mkAE_MARG_MS2016_Cmpt_EstMag0Filter(mkae_ms, v1, mag0_est);
	mkAE_MARG_MS2016_Cmpt_MesgyroFilter(mkae_ms, gyro, gyro_f);
	mkAE_MARG_MS2016_Cmpt_MesAccFilter(mkae_ms, acc, acc_f);
	mkAE_MARG_MS2016_Cmpt_MesMagFilter(mkae_ms, mag, mag_f);	

	mkAE_MARG_MahonyECF_Quat_Core(&(mkae_ms->mkae), gyro_f, acc_f, acc0_est, mag_f, mag0_est, wA, wM, Kp, Ki, dt);
}
/* Outputs the attitude matrix */
void __attribute__((optimize("O0"))) mkAE_MARG_MS2016_CF_AM(mkAE_MARG_MS2016 *mkae_ms, double *gyro, double *acc, double *mag, double wA, double wM, double Kp, double Ki, double dt){
	double acc0_est[3], mag0_est[3], gyro_f[3], acc_f[3], mag_f[3], v1[3];

	mk_Cmpt_AMT_Vec(mkae_ms->mkae.m_att, mkae_ms->mkae._acc0, v1);
	mkAE_MARG_MS2016_Cmpt_EstAcc0Filter(mkae_ms, v1, acc0_est);
	mk_Cmpt_AMT_Vec(mkae_ms->mkae.m_att, mkae_ms->mkae._mag0, v1);	
	mkAE_MARG_MS2016_Cmpt_EstMag0Filter(mkae_ms, v1, mag0_est);
	mkAE_MARG_MS2016_Cmpt_MesgyroFilter(mkae_ms, gyro, gyro_f);
	mkAE_MARG_MS2016_Cmpt_MesAccFilter(mkae_ms, acc, acc_f);
	mkAE_MARG_MS2016_Cmpt_MesMagFilter(mkae_ms, mag, mag_f);
	
	mkAE_MARG_MahonyECF_AM_Core(&(mkae_ms->mkae), gyro_f, acc_f, acc0_est, mag_f, mag0_est, wA, wM, Kp, Ki, dt);
}



/*****************************************************************************************************
 * IMU
 *****************************************************************************************************/
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
void __attribute__((optimize("O0"))) mkAE_IMU_Initialize_Core(mkAE_IMU *mkae, double m_att[][3], double *q_est, double *gyro_est, double *bias_est, double *acc0){
	int i, j;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++) mkae->m_att[i][j] = m_att[i][j];
		mkae->_bias_est[i] = bias_est[i];
		mkae->_acc0[i] = acc0[i];
		mkae->gyro_est[i] = gyro_est[i];
	}
	for(i=0;i<4;i++) mkae->q_est[i] = q_est[i];
}
void __attribute__((optimize("O0"))) mkAE_IMU_Initialize(mkAE_IMU *mkae, double *acc0){
	int i, j;
	double bias_est[3], m_att[3][3], gyro_est[3], q_est[4];

	for(i=0;i<3;i++){
		for(j=0;j<3;j++) m_att[i][j] = 0.0;
		m_att[i][i] = 1.0;
		bias_est[i] = 0.0;
		gyro_est[i] = 0.0;
		q_est[i+1] = 0.0;
	}
	q_est[0] = 1.0;
	mkAE_IMU_Initialize_Core(mkae, m_att, q_est, gyro_est, bias_est, acc0);
}

/* Reset function of the sturcture (mkAE_IMU) 
 * 	- gyro  : Measured angular velocity with respect to the sensor frame
 *	- acc  : Measured accleration with respect to the sensor frame
 */
void __attribute__((optimize("O0"))) mkAE_IMU_Reset(mkAE_IMU *mkae, double *gyro, double *acc){
	int i, j;
	double bias_est[3], m_att[3][3], q_est[4];

	for(i=0;i<3;i++){
		for(j=0;j<3;j++) m_att[i][j] = 0.0;
		m_att[i][i] = 1.0;
		bias_est[i] = 0.0;
		q_est[i+1] = 0.0;
	}
	q_est[0] = 1.0;
	mkAE_IMU_Initialize_Core(mkae, m_att, q_est, gyro, bias_est, acc);
}

/* Explicit complementary filter for IMU (Mahony et al., 2008)
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
/* Outputs the quaternion */
void __attribute__((optimize("O0"))) mkAE_IMU_MahonyECF_Quat_Core(mkAE_IMU *mkae, double *gyro, double *acc, double *acc0, double wA, double Kp, double Ki, double dt){
	int i;
	double norm=0.0;
	double q1[4];
	double gyro_acc[3], v1[3];

	/* Error vector calculated by Accelerometer and Magnetometer outputs */
	/* Accel */
	mk_Cmpt_OuterProduct(acc, acc0, gyro_acc);
	for(i=0;i<3;i++){
//		v1[i] = wA*gyro_acc[i];
		v1[i] = my_wA[i]*gyro_acc[i];
		mkae->_bias_est[i] = mkae->_bias_est[i]-Ki*dt*v1[i];
	}

	/* Angular velcoity compensation */
	for(i=0;i<3;i++){
		mkae->gyro_est[i] = gyro[i] - (mkae->_bias_est[i]) + Kp*v1[i];
	}

	/* Update the quaternion */
	mk_Cmpt_Quat_Derivative(mkae->q_est, mkae->gyro_est, q1);
	for(i=0;i<4;i++){
		mkae->q_est[i] = mkae->q_est[i] + q1[i]*dt;
		norm = norm + (mkae->q_est[i])*(mkae->q_est[i]);
	}
	for(i=0;i<4;i++) mkae->q_est[i] = (mkae->q_est[i])/sqrt(norm);
	mk_Cmpt_AM_Quat(mkae->q_est, mkae->m_att);
}
void __attribute__((optimize("O0"))) mkAE_IMU_MahonyECF_Quat(mkAE_IMU *mkae, double *gyro, double *acc, double wA, double Kp, double Ki, double dt){
	double acc0[3];
	mk_Cmpt_Rotate_Quat_Vec(mkae->q_est, mkae->_acc0, acc0);
	mkAE_IMU_MahonyECF_Quat_Core(mkae, gyro, acc, acc0, wA, Kp, Ki, dt);
}
/* Outputs the attitude matrix */
void __attribute__((optimize("O0"))) mkAE_IMU_MahonyECF_AM_Core(mkAE_IMU *mkae, double *gyro, double *acc, double *acc0, double wA, double Kp, double Ki, double dt){
	int i, j;
	double m1[3][3], m2[3][3];
	double gyro_acc[3], v1[3];

	/* Error vector calculated by Accelerometer and Magnetometer outputs */
	/* Accel */
	mk_Cmpt_OuterProduct(acc, acc0, gyro_acc);
	for(i=0;i<3;i++){
		v1[i] = wA * gyro_acc[i];
		mkae->_bias_est[i] = mkae->_bias_est[i]-Ki*dt*v1[i];
	}	

	/* Angular velcoity compensation */
	for(i=0;i<3;i++){
		mkae->gyro_est[i] = gyro[i] - (mkae->_bias_est[i]) + Kp*v1[i];
		v1[i] = dt*(mkae->gyro_est[i]);
	} 
	
	/* Update the attitude matrix */
	mk_Cmpt_AM_Rodrigues(m1, v1);
	mk_Cmpt_AM_AM(mkae->m_att, m1, m2);
	for(i=0;i<3;i++){
		for(j=0;j<3;j++) mkae->m_att[i][j] = m2[i][j];
	}
	mk_Cmpt_Quat_AM(mkae->m_att, mkae->q_est);
}
void __attribute__((optimize("O0"))) mkAE_IMU_MahonyECF_AM(mkAE_IMU *mkae, double *gyro, double *acc, double wA, double Kp, double Ki, double dt){
	double acc0[3];
	mk_Cmpt_AMT_Vec(mkae->m_att, mkae->_acc0, acc0);
	mkAE_IMU_MahonyECF_AM_Core(mkae, gyro, acc, acc0, wA, Kp, Ki, dt);
}




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
void __attribute__((optimize("O0"))) mkAE_IMU_MS2016_Initialize_Core(mkAE_IMU_MS2016 *mkae_ms, double m_att[][3], double *q_est, double *gyro_est0, double *bias_est, double *acc_est0, double *acc0, int ord_EstAcc0, int ord_MesAcc, int ord_Mesgyro){
	int i, j;
	
	mkAE_IMU_Initialize_Core(&(mkae_ms->mkae), m_att, q_est, gyro_est0, bias_est, acc0);

	mkae_ms->ord_EstAcc0 = ord_EstAcc0;
	mkae_ms->ord_MesAcc = ord_MesAcc;
	mkae_ms->ord_Mesgyro = ord_Mesgyro;
	
	for(i=0;i<3;i++) mkae_ms->param_EstAcc0Fil[i] = mk_Alloc_Filter_Param(ord_EstAcc0);
	for(i=0;i<3;i++) mkae_ms->_acc0_est[i] = mk_Alloc_Filter_Input(ord_EstAcc0);
	for(i=0;i<3;i++) mkae_ms->_acc0_est_f[i] = mk_Alloc_Filter_Output(ord_EstAcc0);
	
	for(i=0;i<3;i++) mkae_ms->param_MesAccFil[i] = mk_Alloc_Filter_Param(ord_MesAcc);
	for(i=0;i<3;i++) mkae_ms->_acc_mes[i] = mk_Alloc_Filter_Input(ord_MesAcc);
	for(i=0;i<3;i++) mkae_ms->_acc_mes_f[i] = mk_Alloc_Filter_Output(ord_MesAcc);

	for(i=0;i<3;i++) mkae_ms->param_MesgyroFil[i] = mk_Alloc_Filter_Param(ord_Mesgyro);
	for(i=0;i<3;i++) mkae_ms->_gyro_mes[i] = mk_Alloc_Filter_Input(ord_Mesgyro);
	for(i=0;i<3;i++) mkae_ms->_gyro_mes_f[i] = mk_Alloc_Filter_Output(ord_Mesgyro);

	for(i=0;i<3;i++){
		for(j=0;j<mk_Cmpt_Filter_Num(ord_EstAcc0);j++){
			mkae_ms->_acc0_est[i][j] = acc_est0[i];
			mkae_ms->_acc0_est_f[i][j] = acc_est0[i];
		}
		for(j=0;j<mk_Cmpt_Filter_Num(ord_MesAcc);j++){
			mkae_ms->_acc_mes[i][j] = acc_est0[i];
			mkae_ms->_acc_mes_f[i][j] = acc_est0[i];
		}
		for(j=0;j<mk_Cmpt_Filter_Num(ord_Mesgyro);j++){
			mkae_ms->_gyro_mes[i][j] = gyro_est0[i];
			mkae_ms->_gyro_mes_f[i][j] = gyro_est0[i];
		}
	}
}
void __attribute__((optimize("O0"))) mkAE_IMU_MS2016_Initialize(mkAE_IMU_MS2016 *mkae_ms, double *gyro0, double *acc0, int ord_EstAcc0, int ord_MesAcc, int ord_Mesgyro){
	int i, j;
	double bias_est[3], m_att[3][3], q_est[4];
	
	for(i=0;i<3;i++){
		for(j=0;j<3;j++) m_att[i][j] = 0.0;
		m_att[i][i] = 1.0;
		bias_est[i] = 0.0;
		q_est[i+1] = 0.0;
	}
	q_est[0] = 1.0;
	mkAE_IMU_MS2016_Initialize_Core(mkae_ms, m_att, q_est, gyro0, bias_est, acc0, acc0, ord_EstAcc0, ord_MesAcc, ord_Mesgyro);
}

/* Reset function of the sturcture (mkAE_IMU_MS2016) */
void __attribute__((optimize("O0"))) mkAE_IMU_MS2016_Reset(mkAE_IMU_MS2016 *mkae_ms, double *gyro, double *acc){
	int i, j;
	mkAE_IMU_Reset(&(mkae_ms->mkae), gyro, acc);
	for(i=0;i<3;i++){
		for(j=0;j<mk_Cmpt_Filter_Num(mkae_ms->ord_EstAcc0);j++){
			mkae_ms->_acc0_est[i][j] = acc[i];
			mkae_ms->_acc0_est_f[i][j] = acc[i];
		}
		for(j=0;j<mk_Cmpt_Filter_Num(mkae_ms->ord_MesAcc);j++){
			mkae_ms->_acc_mes[i][j] = acc[i];
			mkae_ms->_acc_mes_f[i][j] = acc[i];
		}
		for(j=0;j<mk_Cmpt_Filter_Num(mkae_ms->ord_Mesgyro);j++){
			mkae_ms->_gyro_mes[i][j] = gyro[i];
			mkae_ms->_gyro_mes_f[i][j] = gyro[i];
		}
	}
}


/* mkAE_IMU_MS2016_Free() frees the dynamic memory in the sturcture (mkAE_IMU_MS2016) */
void __attribute__((optimize("O0"))) mkAE_IMU_MS2016_Free(mkAE_IMU_MS2016 *mkae_ms){
	int i;
	for(i=0;i<3;i++){
		free(mkae_ms->param_EstAcc0Fil[i]);
		mkae_ms->param_EstAcc0Fil[i] = NULL;
		free(mkae_ms->_acc0_est[i]);
		mkae_ms->_acc0_est[i] = NULL;
		free(mkae_ms->_acc0_est_f[i]);
		mkae_ms->_acc0_est_f[i] = NULL;
	
		free(mkae_ms->param_MesAccFil[i]);
		mkae_ms->param_MesAccFil[i] = NULL;
		free(mkae_ms->_acc_mes[i]);
		mkae_ms->_acc_mes[i] = NULL;
		free(mkae_ms->_acc_mes_f[i]);
		mkae_ms->_acc_mes_f[i] = NULL;

		free(mkae_ms->param_MesgyroFil[i]);
		mkae_ms->param_MesgyroFil[i] = NULL;
		free(mkae_ms->_gyro_mes[i]);
		mkae_ms->_gyro_mes[i] = NULL;
		free(mkae_ms->_gyro_mes_f[i]);
		mkae_ms->_gyro_mes_f[i] = NULL;
	}
}


/* Filter functions 
 * The arguments of this function are as follows:
 * 	- **    : Input of the filter
 *	- **_f	: Output of the filter
 */
void __attribute__((optimize("O0"))) mkAE_IMU_MS2016_Cmpt_EstAcc0Filter(mkAE_IMU_MS2016 *mkae_ms, double *acc0_est, double *acc0_est_f){
	int i;
	for(i=0;i<3;i++) mk_Cmpt_012_OrderFilter(mkae_ms->ord_EstAcc0, &acc0_est[i], mkae_ms->_acc0_est[i], mkae_ms->_acc0_est_f[i], mkae_ms->param_EstAcc0Fil[i], &acc0_est_f[i]);
}
void __attribute__((optimize("O0"))) mkAE_IMU_MS2016_Cmpt_MesAccFilter(mkAE_IMU_MS2016 *mkae_ms, double *acc_mes, double *acc_mes_f){
	int i;
	for(i=0;i<3;i++) mk_Cmpt_012_OrderFilter(mkae_ms->ord_MesAcc, &acc_mes[i], mkae_ms->_acc_mes[i], mkae_ms->_acc_mes_f[i], mkae_ms->param_MesAccFil[i], &acc_mes_f[i]);
}
void __attribute__((optimize("O0"))) mkAE_IMU_MS2016_Cmpt_MesgyroFilter(mkAE_IMU_MS2016 *mkae_ms, double *gyro_mes, double *gyro_mes_f){
	int i;
	for(i=0;i<3;i++) mk_Cmpt_012_OrderFilter(mkae_ms->ord_Mesgyro, &gyro_mes[i], mkae_ms->_gyro_mes[i], mkae_ms->_gyro_mes_f[i], mkae_ms->param_MesgyroFil[i], &gyro_mes_f[i]);
}



/* Explicit complementary filter with dynamics Compensation for MARG (Masuya&Sugiahra, 2016) 
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
/* Outputs the quaternion */
void __attribute__((optimize("O0"))) mkAE_IMU_MS2016_CF_Quat(mkAE_IMU_MS2016 *mkae_ms, double *gyro, double *acc, double wA, double Kp, double Ki, double dt){
	double acc0_est[3], gyro_f[3], acc_f[3], v1[3];

	mk_Cmpt_Rotate_Quat_Vec(mkae_ms->mkae.q_est, mkae_ms->mkae._acc0, v1);
	mkAE_IMU_MS2016_Cmpt_EstAcc0Filter(mkae_ms, v1, acc0_est);
	mkAE_IMU_MS2016_Cmpt_MesgyroFilter(mkae_ms, gyro, gyro_f);
	mkAE_IMU_MS2016_Cmpt_MesAccFilter(mkae_ms, acc, acc_f);

	mkAE_IMU_MahonyECF_Quat_Core(&(mkae_ms->mkae), gyro_f, acc_f, acc0_est, wA, Kp, Ki, dt);
}
/* Outputs the attitude matrix */
void __attribute__((optimize("O0"))) mkAE_IMU_MS2016_CF_AM(mkAE_IMU_MS2016 *mkae_ms, double *gyro, double *acc, double wA, double Kp, double Ki, double dt){
	double acc0_est[3], gyro_f[3], acc_f[3], v1[3];

	mk_Cmpt_AMT_Vec(mkae_ms->mkae.m_att, mkae_ms->mkae._acc0, v1);
	mkAE_IMU_MS2016_Cmpt_EstAcc0Filter(mkae_ms, v1, acc0_est);
	mkAE_IMU_MS2016_Cmpt_MesgyroFilter(mkae_ms, gyro, gyro_f);
	mkAE_IMU_MS2016_Cmpt_MesAccFilter(mkae_ms, acc, acc_f);
	
	mkAE_IMU_MahonyECF_AM_Core(&(mkae_ms->mkae), gyro_f, acc_f, acc0_est, wA, Kp, Ki, dt);
}

