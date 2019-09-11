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

void U_PUT_TypeDouble_CSV(double * array,int length,int ap);
void U_PUT_TypeHEX_ADD_CSUM_CSV(u32 * array,int length);
void AutoBiasUpdate_START(void);
void AutoBiasUpdate_STOP(void);

static double CTRL_Cycle = 0.001;
static double SEND_Cycle = 0.001;
static u32 SendCntPeriod =10;	// Counter for delaying transmission cycle
static int ABU_Cnt =120*2;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim2;
extern bool CMD_PARSE_FLAG;

static double yaw,pitch,roll;
static bool PoseUpdateFlg =false;
static bool SendEnableFlg =false;
static bool AutoBiasUpdateFlg =false;
static u8 StringFormat =YawPitchRoll;

extern FLASH_ROM_Parameters Params;

// Acquisition of CONF switch information
u8 GET_CONF_SW(void){
	u8 buf = 0;
	if(CSW1_STAT)
		buf |=CSW1_MASK;

	if(CSW2_STAT)
		buf |=CSW2_MASK;

	if(CSW3_STAT)
		buf |=CSW3_MASK;

	if(CSW4_STAT)
		buf |=CSW4_MASK;

	if(CSW5_STAT)
		buf |=CSW5_MASK;

	if(CSW6_STAT)
		buf |=CSW6_MASK;

	if(CSW7_STAT)
		buf |=CSW7_MASK;

	if(CSW8_STAT)
		buf |=CSW8_MASK;

	return buf;
}

u8 IMU_INIT(double c_cycle,double s_cycle){
  // Acquisition of CONF switch information
  u8 conf_sw = GET_CONF_SW();

  //When USB to UART is selected, all settings of IMU are skipped
  if((conf_sw & CSW_MODE_MASK) ==USB_to_UART_MODE){
	  return conf_sw;
  }

  // USB or UART selection
  if(!(conf_sw & CSW6_MASK)){
	  USB_Select();
  }else{
	  UART_Select();
  }

  CTRL_Cycle = c_cycle;
  SEND_Cycle = s_cycle;

  // Send counter calculation
  SendCntPeriod =(s32)((SEND_Cycle*1000.0)/(CTRL_Cycle*1000.0))-1; //10ms

  // IMU initialization
  ADIS_INIT();

  // Bias_Correction_Update ON / OFF
  if(!(conf_sw & CSW5_MASK)){// In the case of OFF
	  LD2_ON;
	  AutoBiasUpdateFlg=true;
	  AutoBiasUpdate_START();
  }else{
	  AutoBiasUpdateFlg=false;
  }

  return conf_sw;
}

void pose_update_func(){
	// Acquisition of sensor value and calculation of attitude angle.
	mkAE_UpdateAngle(&yaw,&pitch,&roll);

	// Output according to the set transmission cycle.
	static int i=0;
	if(SendEnableFlg){
		if(++i >SendCntPeriod){
			if(PoseUpdateFlg){
				LD3_ON;
			}else{
				LD3_OFF;
			}

			PoseUpdateFlg =true;
			i=0;
		}
	}else{
		i=0;
		PoseUpdateFlg =false;
	}
}

// Update and transmit attitude angles.
void print_func(void)
{
	// Not sent during command processing
	if(CMD_PARSE_FLAG){
		PoseUpdateTermination();
	}

	// transmit attitude angles.
	if(IsPoseUpdate()){
		// The mode is branched by the CONF switch.
		switch(StringFormat){

		// attitude angle output mode
		case YawPitchRoll:
		{
			double arr[3]={yaw,pitch,roll};
			U_PUT_TypeDouble_CSV(arr,3,3);
			break;
		}

		// Output mode of angular velocity
		case GYRO_Degree:
		{
			double gyro_raw[3],acc_raw[3];
			Get_RawData(gyro_raw,acc_raw);

			double arr[3];
			for(int i=0;i<3;i++){
				arr[i] =gyro_raw[i]*180.0/M_PI;
			}

			U_PUT_TypeDouble_CSV(arr,3,3);
			break;
		}

		// This mode transmits raw data of sensor values as a character string.
		case GYRO_ACC_HexData:
		{
			u32 gyro_hex[3],acc_hex[3];
			Get_HexData(gyro_hex,acc_hex);

			u32 arr[6];
			for(int i =0;i<3;i++){
				arr[i] =gyro_hex[i];
				arr[i+3] =acc_hex[i];
			}

			U_PUT_TypeHEX_ADD_CSUM_CSV(arr,6);
			break;
		}

		// Mode to transmit raw data of sensor values in binary.
		case GYRO_ACC_BinaryData:
		{
			u32 gyro_hex[3],acc_hex[3];
			Get_HexData(gyro_hex,acc_hex);

			u32 arr[6];
			for(int i =0;i<3;i++){
				arr[i] =gyro_hex[i];
				arr[i+3] =acc_hex[i];
			}

			char sb[128];
			memset(sb,0,sizeof(sb));

			u8 sb_len =0;
			sb[sb_len++] = 0xAC;
			sb[sb_len++] = 0xCA;
			sb[sb_len++] = 24; //length
			for(int i =0;i<6;i++){
				sb[sb_len++] =(arr[i]>>24)&0xff;
				sb[sb_len++] =(arr[i]>>16)&0xff;
				sb[sb_len++] =(arr[i]>>8)&0xff;
				sb[sb_len++] =(arr[i])&0xff;
			}

			u32 sum=0;
			for(int i =2;i<sb_len;i++){
				sum+=sb[i];
			}
			sb[sb_len++]=sum&0xff;
			U_putd(sb,(char)sb_len);
			break;
		}

		// Mode to transmit attitude angle and acceleration values.
		case YawPitchRoll_ACC:
		{
			double gyro_raw[3],acc_raw[3];
			Get_RawData(gyro_raw,acc_raw);

			double arr[6]={yaw,pitch,roll,0,0,0};
			for(int i=3;i<6;i++){
				arr[i] =acc_raw[i-3];
			}

			U_PUT_TypeDouble_CSV(arr,6,3);
			break;
		}

		case GYRO_ACC_TEMP:
		{
			double gyro_raw[3],acc_raw[3];
			Get_RawData(gyro_raw,acc_raw);

			double arr[7]={0,0,0,0,0,0};
			for(int i=0;i<3;i++){
				arr[i] =gyro_raw[i];
				arr[i+3] =acc_raw[i];
			}

			arr[6] =Get_Temp();

			U_PUT_TypeDouble_CSV(arr,7,3);
			break;
		}

		}

		PoseUpdateTermination();
	}
}

// Send the 16-bits register in CSV format.
void U_PUT_TypeHEX_ADD_CSUM_CSV(u32 * array,int length){
	char buf[128];
	memset(buf,0,sizeof(buf));

	for(int i =0;i<length;i++){
		str_concat(buf,str_putx(array[i],8));

		str_concat(buf,",");
	}

	u32 sum=0;
	for(int i =0;i<length;i++){
		sum +=(array[i]>>24)&0xff;
		sum +=(array[i]>>16)&0xff;
		sum +=(array[i]>>8)&0xff;
		sum +=(array[i])&0xff;
	}

	str_concat(buf,str_putx(sum&0xff,2));

	str_concat(buf,"\r\n");
	U_puts(buf);
}

// Send double in CSV format.
void U_PUT_TypeDouble_CSV(double * array,int length,int ap){
	char buf[128];
	memset(buf,0,sizeof(buf));

	for(int i =0;i<length;i++){
		str_concat(buf,str_putlf(array[i],ap));

		if(i<(length-1)){
			str_concat(buf,",");
		}
	}
	str_concat(buf,"\r\n");
	U_puts(buf);
}

// Get transmission start flag
bool IsPoseUpdate(){
	return PoseUpdateFlg;
}

// Reset transmission start flag
void PoseUpdateTermination(){
	PoseUpdateFlg =false;
}

// Transmission cycle setting function
void Set_SendCycle(double s_cycle){
	SEND_Cycle = s_cycle;
	SendCntPeriod =(s32)((SEND_Cycle*1000.0)/(CTRL_Cycle*1000.0))-1; //10ms
}

bool IsSendEnable(void){
	return SendEnableFlg;
}

void SendStart(void){
	SendEnableFlg =true;
}

void SendStop(void){
	SendEnableFlg =false;
}

void Set_Format(u8 format){
	StringFormat =format;
}

u8 Get_Format(void){
	return StringFormat;
}

double Get_CtrlCycle(void){
	return CTRL_Cycle;
}

void AutoBiasUpdate_TIM(){
	if(--ABU_Cnt <= 0){
		LD2_OFF;
		ADIS_Bias_Correction_Update();
		AutoBiasUpdateFlg = false;
		AutoBiasUpdate_STOP();
	}else{
		// When the user switch is pressed, standby is forcibly terminated.
		if(READ_USER_SW){
			LD2_OFF;
			ADIS_Bias_Correction_Update();
			AutoBiasUpdate_STOP();
			HAL_Delay(500);
			AutoBiasUpdateFlg = false;
		}
	}
}

bool IsAutoBiasUpdate(){
	return AutoBiasUpdateFlg;
}

int AutoBiasUpdate_TimeLeft(){
	return ABU_Cnt/2;
}

void AutoBiasUpdate_START(void){
	ABU_Cnt = Params.startup_time_s*2;
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	HAL_TIM_Base_Start_IT(&htim2);
}

void AutoBiasUpdate_STOP(void){
	HAL_TIM_Base_Stop_IT(&htim2);
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
}
