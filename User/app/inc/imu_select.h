/*
 * adis_select.h
 *
 *  Created on: 2025/06/11
 *      Author: techno-road
 */

#ifndef IMU_INC_IMU_SELECT_H_
#define IMU_INC_IMU_SELECT_H_

#define MDL_1BMLZ  0x0003
#define MDL_2BMLZ  0x0007
#define MDL_3BMLZ  0x000F

bool IMU_Initialization(void);
void IMU_Update6AxisData();
void IMU_GetSensitivity(double *GyroSensi, double *AccSensi);
bool IMU_IsBurstReadCommand(uint16_t addr);

#endif /* IMU_INC_ADIS_SELECT_H_ */
