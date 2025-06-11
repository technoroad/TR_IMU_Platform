/*
 * adis_type2_ctrl.h
 *
 *  Created on: 2025/06/19
 *      Author: techno-road
 */

#ifndef IMU_INC_ADIS_TYPE2_CTRL_H_
#define IMU_INC_ADIS_TYPE2_CTRL_H_

//==============================================================================
// PAGE 0 address
//==============================================================================
#define TYPE2_PAGE_ID       0x00
#define TYPE2_DATA_CNT      0x04
#define TYPE2_SYS_E_FLAG    0x08
#define TYPE2_TEMP_OUT      0x1C
#define TYPE2_PROD_CMD      0x7E
#define TYPE2_Z_ACCL_OUT    0x26


//==============================================================================
// PAGE 2 address
//==============================================================================
#define TYPE2_XG_BIAS_LOW   0x10
#define TYPE2_ZG_BIAS_HIGH  0x1A
#define TYPE2_ZA_BIAS_HIGH  0x26
//#define TYPE2_X_GYRO_LOW    0x04


//==============================================================================
// PAGE 3 address
//==============================================================================
#define TYPE2_GLOB_CMD 0x02
#define TYPE2_MSC_CTRL 0x0A
#define TYPE2_DEC_RATE 0x0C
#define TYPE2_NULL_CNFG 0x0E
#define TYPE2_RANG_MDL 0x12
#define TYPE2_FILTR_BNK_0 0x16
#define TYPE2_FILTR_BNK_1 0x18
#define TYPE2_FIRM_REV 0x78

#define TYPE2_BURST_CMD 0x7C
#define TYPE2_BURST_CMD_SIZE 21

#define PAGE0 0
#define PAGE1 1
#define PAGE2 2
#define PAGE3 3

#define FILT_BANK_A 0
#define FILT_BANK_B 1
#define FILT_BANK_C 2
#define FILT_BANK_D 3
#define FILTER_OFF (int)-1

void IMU_RegisterType2Functions();

#endif /* IMU_INC_ADIS_TYPE2_CTRL_H_ */
