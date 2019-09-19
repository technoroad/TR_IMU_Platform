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


#include "select_sensor.h"
# if defined(ADIS16470)

#include <libraries.h>
#include "math.h"

// Save raw data of angular velocity and acceleration
static vu32 gyro_hex[3],acc_hex[3];
static double gyro_raw[3],acc_raw[3];

// Delay per data of SPI communication [us]
static u32 tSTALL =17;

static double GyroSensitivity = 655360.0;
static double AcclSensitivity = 52428800.0;

// IMU initialization
void ADIS_INIT(void){
	// Hardware reset
	LD1_ON;
	HAL_GPIO_WritePin(ADIS_nRST_GPIO_Port,ADIS_nRST_Pin,GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(ADIS_nRST_GPIO_Port,ADIS_nRST_Pin,GPIO_PIN_SET);
	HAL_Delay(1000);
	LD1_OFF;

	// Check if specified sensor is connected
	int i;
	for(i=0;i<10;i++){

		if(ADIS_VerificationProductId()){
			break;
		}
		HAL_Delay(1);
	}

	if(i ==10){
		// Stop here if different sensor
		while(1){
			LD3_ON;
			U_puts("ERROR_PRODUCT_ID_INCORRECT\r\n");
			HAL_Delay(500);
			LD3_OFF;
			HAL_Delay(500);
		}
	}

	//Hard filter setting in sensor
	ADIS_HardwareFilterSelect(1);
}

// Check if specified sensor is connected
u8 ADIS_VerificationProductId(void){
	u16 id =ADIS_Bloking_RegRead(PROD_ID);

	if(id != PRODUCT_ID){
		return false;
	}else{
		return true;
	}
}

// Update sensor bias value
void ADIS_Bias_Correction_Update(void){
	ADIS_RegWrite(GLOB_CMD, 0x0001);
}

// Write to any address value
void ADIS_WRITE_REG(u16 addr,u16 value){
	ADIS_RegWrite(addr,value);
}

// Read any address value
u16 ADIS_READ_REG(u16 addr){

	// Burst command can not be read
	if(addr == BURST_CMD)return false;

	u16 val =ADIS_Bloking_RegRead(addr);

	return val;
}

// Read all register values
u16 ADIS_PAGE_DUMP(u16 * vals){

	u16 len=0;
	for(int i=0;i<=0x7E;i+=2){
		if(i == BURST_CMD){ // Burst command can not be read
			vals[len] =0;
		}else{
			vals[len] = ADIS_Bloking_RegRead(i);
		}
		len++;
	}

	return len;
}


//Hard filter selection.
//See the data sheet for details.
bool ADIS_HardwareFilterSelect(int dat) {
	ADIS_RegWrite(FILT_CTRL, dat);
	return true;
}

// Read the acceleration, gyro, and temperature values
void ADIS_32bit_READ(uint16_t *rb){
	int j =0;
	for(int i=0x04;i<=0x1C;i+=2){
		rb[j++] = ADIS_NoBloking_RegRead(i);
	}
	rb[j++] = ADIS_NoBloking_RegRead(0x00);
}

// Get acceleration and angular velocity from the value read by ADIS_32bit_READ()
int ADIS_32bitIMUConvert(u16 *buf,double *gyro,double *acc){
	//Save raw data of angular velocity and acceleration
	for(int i=0; i<3;i++){
		gyro_hex[i] =((buf[i*2+2] << 16) | buf[i*2+1]);
		gyro[i] =(s32)gyro_hex[i] / GyroSensitivity*M_PI/180.0;
		gyro_raw[i] =gyro[i];

		acc_hex[i] =((buf[i*2+8] << 16) | buf[i*2+7]);
		acc[i] =(s32)acc_hex[i] / AcclSensitivity;
		acc_raw[i] =acc[i];
		acc[i] =acc[i] *G_ACCL;
	}

	double n = sqrt(acc[0]*acc[0]+acc[1]*acc[1]+acc[2]*acc[2]);
	if(n>0.0001){
		acc[0]/=n;
		acc[1]/=n;
		acc[2]/=n;
	}else{
		acc[0]=0;
		acc[1]=0;
		acc[2]=0;
	}
	return(1);
}

// Stores the acquired acceleration and angular velocity binary values in variables.
void ADIS_GetHexData(u32 *p_gyro, u32 *p_acc){
	for(int i=0;i<3;i++){
		p_gyro[i] =gyro_hex[i];
		p_acc[i] = acc_hex[i];
	}
}

// Stores the acquired acceleration and angular velocity values in variables.
void ADIS_GetRawData(double *p_gyro, double *p_acc){
	for(int i=0;i<3;i++){
		p_gyro[i] =gyro_raw[i];
		p_acc[i] = acc_raw[i];
	}
}

// Get temperature from the value read by ADIS_32bit_READ()
double ADIS_16bitTempConvert(u16 *buf){
	return (double)((int16_t)buf[13])*0.1;
}

// SPI reading on blocking
u16 ADIS_Bloking_RegRead(u8 addr){
	if(addr >0x7F)return 0;

	u16 rbuf=0;

	SPI_enable_nss();
	SPI_write_word(addr<<8);
	SPI_disable_nss();

	delay_us(tSTALL);

	SPI_enable_nss();
	SPI_read_words(&rbuf,1);
	SPI_disable_nss();

	delay_us(tSTALL);

	return rbuf;
}

// SPI read without blocking
u16 ADIS_NoBloking_RegRead(u8 addr){
	if(addr >0x7F)return 0;

	u16 rb=0;
	SPI_enable_nss();
	SPI_NonBloking_wr_word(addr<<8,&rb);
	SPI_disable_nss();

	delay_us(tSTALL);

	return rb;
}

// Write to register
int ADIS_RegWrite(u8 regAddr, s16 regData){
	// Write register address and data
	uint16_t addr = (((regAddr & 0x7F) | 0x80) << 8); // Toggle sign bit, and check that the address is 8 bits
	uint16_t lowWord = (addr | (regData & 0xFF)); // OR Register address (A) with data(D) (AADD)
	uint16_t highWord = ((addr | 0x100) | ((regData >> 8) & 0xFF)); // OR Register address with data and increment address

	// Write highWord to SPI bus
	SPI_enable_nss();
	SPI_write_word(lowWord);
	SPI_disable_nss();

	delay_us(tSTALL);

	SPI_enable_nss();
	SPI_write_word(highWord);
	SPI_disable_nss();

	delay_us(tSTALL);

	return(1);
}

// Get sensor sensitivity
void ADIS_Get_SensSensi(double *GyroSensi,double *AccSensi){
	*GyroSensi = GyroSensitivity;
	*AccSensi = AcclSensitivity;
}

#endif
