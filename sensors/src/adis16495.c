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
# if defined(ADIS16495)

#include <libraries.h>
#include "math.h"

// decimation settings
#define DECIMATION 1
// Please enter the frequency of the Ready pin.
#define RDY_FREQ (4250.0/(double)(DECIMATION+1)) 	//[Hz]

// Save raw data of angular velocity and acceleration
static vu32 gyro_hex[3],acc_hex[3];
static double gyro_raw[3],acc_raw[3];

// Delay per data of SPI communication [us]
static u32 tSTALL =2;

#if defined(_1BMLZ)
	static double GyroSensitivity = 10485760.0;
	static double AcclSensitivity = 262144000.0;
#elif defined(_2BMLZ)
	static double GyroSensitivity = 2621440.0;
	static double AcclSensitivity = 262144000.0;
#elif defined(_3BMLZ)
	static double GyroSensitivity = 655360.0;
	static double AcclSensitivity = 262144000.0;
#endif

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

	// Set RDY speed
	ADIS_SetDecimation(DECIMATION);

	// Hard filter setting in sensor
	ADIS_HardwareFilterSelect(
			FILT_BANK_C,FILT_BANK_C,FILT_BANK_C,FILT_BANK_C,FILT_BANK_C,FILT_BANK_C);
}

// Check if specified sensor is connected
u8 ADIS_VerificationProductId(void){
	ADIS_RegWrite(PAGE_ID,PAGE0);
	u16 id =ADIS_Bloking_RegRead(PAGE0_PROD_ID);

	if(id != PRODUCT_ID){
		return false;
	}else{
		return true;
	}
}

// Update sensor bias value
void ADIS_Bias_Correction_Update(void){
	ADIS_RegWrite(PAGE_ID,PAGE3);
	ADIS_RegWrite(PAGE3_GLOB_CMD, 0x0001);
	ADIS_RegWrite(PAGE_ID,PAGE0);
}

// Write to any address value
void ADIS_WRITE_REG(u8 page,u16 addr,u16 value){
	if(page >3)return;

	ADIS_RegWrite(PAGE_ID,page);
	ADIS_RegWrite(addr,value);
	ADIS_RegWrite(PAGE_ID,PAGE0);
}

// Read any address value
u16 ADIS_READ_REG(u8 page,u16 addr){
	// Burst command can not be read
	if(page ==PAGE0 && addr == PAGE0_BURST_CMD)return false;

	ADIS_RegWrite(PAGE_ID,page);
	u16 val =ADIS_Bloking_RegRead(addr);
	ADIS_RegWrite(PAGE_ID,PAGE0);

	return val;
}

// Burst command can not be read
void ADIS_BIAS_READ(u32 *bias){
	u16 rbuf[6];
	u16 len =0;

	ADIS_RegWrite(PAGE_ID,PAGE2);

	for(int i=0x10;i<=0x1A;i+=2){
		rbuf[len] = ADIS_Bloking_RegRead(i);
		len++;
	}

	for(int i=0; i<3;i++){
		bias[i] =((rbuf[i*2+1] << 16) | rbuf[i*2]);
	}

	ADIS_RegWrite(PAGE_ID,PAGE0);
}

// Read all register values
u16 ADIS_PAGE_DUMP(u8 page,u16 * vals){

	u16 len=0;
	ADIS_RegWrite(PAGE_ID,page);

	for(int i=0;i<=0x7E;i+=2){
		if(page ==PAGE0 && i == PAGE0_BURST_CMD){
			vals[len] =0;
		}else{
			vals[len] = ADIS_Bloking_RegRead(i);
		}
		len++;
	}

	ADIS_RegWrite(PAGE_ID,PAGE0);

	return len;
}


//Hard filter selection.
//See the data sheet for details.
bool ADIS_HardwareFilterSelect(int gx_f, int gy_f, int gz_f, int ax_f, int ay_f, int az_f) {
	u16 filt_bnk0 = 0;
	u16 filt_bnk1 = 0;

	int bank0_arr[5] = { gx_f,gy_f,gz_f,ax_f,ay_f };

	//FILTR_BNK_0
	for (int i = 0; i < 5; i++) {
		if ((bank0_arr[i] <= FILT_BANK_D) && (bank0_arr[i] >= FILT_BANK_A)) {
			filt_bnk0 |= (u16)((bank0_arr[i] | 0x0004) << (i*3));
		}else{
			filt_bnk0 |= (u16)((bank0_arr[i] & 0xFFFB) << (i*3));
		}
	}

	//FILTR_BNK_1
	if ((az_f <= FILT_BANK_D) && (az_f >= FILT_BANK_A)) {
		filt_bnk1 |= (u16)(az_f | 0x0004);
	}else{
		filt_bnk1 |= (u16)(az_f & 0xFFFB);
	}

	ADIS_RegWrite(PAGE_ID,PAGE3);
	ADIS_RegWrite(PAGE3_FILTR_BNK_0,filt_bnk0);
	ADIS_RegWrite(PAGE3_FILTR_BNK_1,filt_bnk1);
	ADIS_RegWrite(PAGE_ID,PAGE0);

	return true;
}

// Read the acceleration, gyro, and temperature values
void ADIS_32bit_READ(uint16_t *rb){
	SPI_enable_nss();
	SPI_write_word((u16)PAGE0_BURST_CMD<<8);
	SPI_read_words(rb,18);
	SPI_disable_nss();
}

// Get acceleration and angular velocity from the value read by ADIS_32bit_READ()
int ADIS_32bitIMUConvert(u16 *buf,double *gyro,double *acc){
	//Save raw data of angular velocity and acceleration
	for(int i=0; i<3;i++){
		gyro_hex[i] =((buf[i*2+4] << 16) | buf[i*2+3]);
		gyro[i] =(s32)gyro_hex[i] / GyroSensitivity*M_PI/180.0;
		gyro_raw[i] =gyro[i];

		acc_hex[i] =((buf[i*2+10] << 16) | buf[i*2+9]);
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
	return 25.0 + (double)((s16)buf[2])/80.0;
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

void ADIS_SetDecimation(u16 dec_rate){
	ADIS_RegWrite(PAGE_ID,PAGE3);
	ADIS_RegWrite(PAGE3_DEC_RATE, dec_rate);
	ADIS_RegWrite(PAGE_ID,PAGE0);
}

double ADIS_GetRDY_Freq(){
	return RDY_FREQ;
}

#endif
