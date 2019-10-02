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
#include "string.h"
#include "stdlib.h"

//buffer
#define CMD_BUF_MAX (128)
static char CmdBuf[CMD_BUF_MAX]; //command buffer
static int CmdBuf_wp=0; //Number of stored in command buffer
static u8 len = 0;

#define MAX_LEN  64
static char * words[MAX_LEN];
static char *delim = ",";

bool CMD_PARSE_FLAG =false;
extern FLASH_ROM_Parameters Params;

static void START_BIAS_CORRECTION_func();
static bool WRITE_REG_func();
static bool READ_REG_func();
static bool PAGE_DUMP_func();
static bool HARD_FILTER_SELECT_func();
static bool READ_TEMP_func();
static bool SET_KP_KI_func();
static bool RESET_FILTER_func();
static void other_func();

static bool LOAD_INIT_func();
static bool SAVE_PARAM_func();
static bool DUMP_PARAM_func();
static bool SET_SEND_CYCLE_func();
static bool GET_VERSION_func();
static bool GET_PROD_ID_func();
static bool GET_FORMAT_func();
static bool GET_SENSI_func();
static bool GET_BOARD_NAME_func();
static bool GET_STATUS_func();
static bool SET_STARTUP_TIME_func();

//Command parse processing
void StringCmdParse( char c){
	CMD_PARSE_FLAG =true;

	if (c == '\r'){
		//ignore '\r'
		return;
	}

	if(c != '\n'){
		CmdBuf[CmdBuf_wp++] = c;
	}else{
		CmdBuf[CmdBuf_wp] = '\0';
	}

	len=0;

	// If it exceeds the maximum, I'll go back first
	if (CmdBuf_wp >= CMD_BUF_MAX-1){
		CmdBuf_wp=0;
		return;
	}

	// Start command parsing when line feed code is received
	if(c == '\n'){
		char CmdCpy[CMD_BUF_MAX];
		strcpy(CmdCpy, CmdBuf);

		//Split string by ','
		char *str = strtok(CmdCpy, delim);
		while (str) {
			words[len] = str;
			len++;
			str = strtok(NULL, delim);
		}

		// Exit if there is no received character.
		if(len ==0)words[0] ="";

		//Branch by header character
		if(strcmp(words[0] ,"START_BIAS_CORRECTION") ==0){
			START_BIAS_CORRECTION_func();
		}else if(strcmp(words[0] ,"WRITE_REG") ==0){
			if(!WRITE_REG_func()){
				U_puts("ERROR_WRITE_REG\r\n");
			}
		}else if(strcmp(words[0] ,"READ_REG") ==0){
			if(!READ_REG_func()){
				U_puts("ERROR_READ_REG\r\n");
			}
		}else if(strcmp(words[0] ,"PAGE_DUMP") ==0){
			if(!PAGE_DUMP_func()){
				U_puts("ERROR_PAGE_DUMP\r\n");
			}
		}else if(strcmp(words[0] ,"HARD_FILTER_SELECT") ==0){
			if(!HARD_FILTER_SELECT_func()){
				U_puts("ERROR_HARD_FILTER_SELECT\r\n");
			}
		}else if(strcmp(words[0] ,"READ_TEMP") ==0){
			if(!READ_TEMP_func()){
				U_puts("ERROR_READ_TEMP\r\n");
			}
		}else if(strcmp(words[0] ,"SET_KP_KI") ==0){
			if(!SET_KP_KI_func()){
				U_puts("ERROR_SET_KP_KI\r\n");
			}
		}else if(strcmp(words[0] ,"RESET_FILTER") ==0){
			if(!RESET_FILTER_func()){
				U_puts("ERROR_RESET_FILTER\r\n");
			}
		}else if(strcmp(words[0] ,"LOAD_INIT") ==0){
			if(!LOAD_INIT_func()){
				U_puts("ERROR_LOAD_INIT\r\n");
			}
		}else if(strcmp(words[0] ,"SAVE_PARAM") ==0){
			if(!SAVE_PARAM_func()){
				U_puts("ERROR_SAVE_PARAM\r\n");
			}
		}else if(strcmp(words[0] ,"DUMP_PARAM") ==0){
			if(!DUMP_PARAM_func()){
				U_puts("ERROR_DUMP_PARAM\r\n");
			}
		}else if(strcmp(words[0] ,"GET_VERSION") ==0){
			if(!GET_VERSION_func()){
				U_puts("ERROR_GET_VERSION\r\n");
			}
		}else if(strcmp(words[0] ,"SET_SEND_CYCLE") ==0){
			if(!SET_SEND_CYCLE_func()){
				U_puts("ERROR_SET_SEND_CYCLE\r\n");
			}
		}else if(strcmp(words[0] ,"GET_PROD_ID") ==0){
			if(!GET_PROD_ID_func()){
				U_puts("ERROR_GET_PROD_ID\r\n");
			}
		}else if(strcmp(words[0] ,"GET_FORMAT") ==0){
			if(!GET_FORMAT_func()){
				U_puts("ERROR_GET_FORMAT\r\n");
			}
		}else if(strcmp(words[0] ,"GET_SENSI") ==0){
			if(!GET_SENSI_func()){
				U_puts("ERROR_GET_SENSI\r\n");
			}
		}else if(strcmp(words[0] ,"GET_BOARD_NAME") ==0){
			if(!GET_BOARD_NAME_func()){
				U_puts("ERROR_GET_BOARD_NAME\r\n");
			}
		}else if(strcmp(words[0] ,"GET_STATUS") ==0){
			if(!GET_STATUS_func()){
				U_puts("ERROR_STATUS\r\n");
			}
		}else if(strcmp(words[0] ,"SET_STARTUP_TIME") ==0){
			if(!SET_STARTUP_TIME_func()){
				U_puts("ERROR_STARTUP_TIME\r\n");
			}
		}else if(strcmp(words[0] ,"start") ==0){
			SendStart();
			U_puts("start\r\n");
		}else if(strcmp(words[0] ,"stop") ==0){
			SendStop();
			U_puts("stop\r\n");
		}else if(strcmp(words[0] ,"") ==0){
		}else{
			other_func();
		}

		// Initialize
		CmdBuf_wp=0;
		memset(CmdBuf,0,sizeof(CmdBuf));
	}
	CMD_PARSE_FLAG=false;
}

// Process received data as a character string.
void ReadStringCmd(){
	// If received, it is stored in the buffer.
	if(U_get_cap()!=0){
		// Get received character from ring buffer.
		char buf = U_getc();

		//Command parse
		StringCmdParse(buf);
	}
}

void START_BIAS_CORRECTION_func(){
	ADIS_Bias_Correction_Update();
	U_puts("START_BIAS_CORRECTION\r\n");
}

bool WRITE_REG_func(){
	if(len != 4)return false;
	if(!IsHexString(words[1]))return false;
	if(!IsHexString(words[2]))return false;
	if(!IsHexString(words[3]))return false;

	u16 page =HexStringToU16(words[1]);
	u16 addr =HexStringToU16(words[2]);
	u16 value=HexStringToU16(words[3]);

#if defined(ADIS16495)
	ADIS_WRITE_REG(page,addr,value);
#else
	ADIS_WRITE_REG(addr,value);
#endif

	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"WRITE_REG,");
	str_concat(buf,str_putx(page,2));str_concat(buf,",");
	str_concat(buf,str_putx(addr,2));str_concat(buf,",");
	str_concat(buf,str_putx(value,4));
	str_concat(buf,"\r\n");
	U_puts(buf);

	return true;
}

bool READ_REG_func(){
	if(len != 3)return false;
	if(!IsHexString(words[1]))return false;
	if(!IsHexString(words[2]))return false;

	u16 page =HexStringToU16(words[1]);
	u16 addr =HexStringToU16(words[2]);

#if defined(ADIS16495)
	// Can not read page 0 register 0x7C
	if(page ==PAGE_ID && addr == PAGE0_BURST_CMD)return false;
	u16 value =ADIS_READ_REG(page,addr);
#else
	if(addr == BURST_CMD)return false;
	u16 value =ADIS_READ_REG(addr);
#endif

	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"READ_REG,");
	str_concat(buf,str_putx(page,2));str_concat(buf,",");
	str_concat(buf,str_putx(addr,2));str_concat(buf,",");
	str_concat(buf,str_putx(value,4));
	str_concat(buf,"\r\n");
	U_puts(buf);

	return true;
}

bool PAGE_DUMP_func(){
	if(len != 2)return false;
	if(!IsHexString(words[1]))return false;

	u16 page =HexStringToU16(words[1]);

	u16 vals[64];
	memset(vals,0,sizeof(vals));
#if defined(ADIS16495)
	u16 page_len =ADIS_PAGE_DUMP(page,vals);
#else
	u16 page_len =ADIS_PAGE_DUMP(vals);
#endif

	char buf[1024];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"PAGE_DUMP,");
	str_concat(buf,str_putx(page,2));str_concat(buf,",");

	for(int i=0;i<page_len;i++){
		str_concat(buf,str_putx(vals[i],4));
		if(i<(page_len -1)){
			str_concat(buf,",");
		}
	}
	str_concat(buf,"\r\n");
	U_puts(buf);

	return true;
}

bool HARD_FILTER_SELECT_func(){
	if(len != 7)return false;

	int array[6];
	for(int i=0;i<6;i++){
		if(!IsDecString(words[i+1]))return false;
		array[i] =atoi(words[i+1]);
	}

#if !defined(ADIS1647X)
	ADIS_HardwareFilterSelect(
			array[0],array[1],array[2],array[3],array[4],array[5]);
#else
	if(array[0] ==FILTER_OFF){
		ADIS_HardwareFilterSelect(0);
	}else{
		ADIS_HardwareFilterSelect(array[0] & 0x07);
	}
#endif

	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"HARD_FILTER_SELECT,");
	for(int i=0;i<6;i++){
		str_concat(buf,str_putn(array[i],2));
	}
	str_concat(buf,"\r\n");
	U_puts(buf);

	return true;
}

bool READ_TEMP_func(){
	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"READ_TEMP,");
	str_concat(buf,str_putlf(Get_Temp(),5));
	str_concat(buf,"\r\n");
	U_puts(buf);

	return true;
}

bool SET_KP_KI_func(){
#ifdef MASUYA
	if(len != 3)return false;

	if(!IsDoubleString(words[1]))return false;
	if(!IsDoubleString(words[2]))return false;

	Params.gain_p = DoubleStringToDouble(words[1]);
	Params.gain_i = DoubleStringToDouble(words[2]);

	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"SET_KP_KI,");
	str_concat(buf,str_putlf(Params.gain_p ,5));str_concat(buf,",");
	str_concat(buf,str_putlf(Params.gain_i ,5));
	str_concat(buf,"\r\n");
	U_puts(buf);
#endif

	return true;
}

bool RESET_FILTER_func(){
	mkAE_Reset_Filter();

	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"RESET_FILTER\r\n");
	U_puts(buf);

	return true;
}

static bool LOAD_INIT_func(){
	Params_initialize();

	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"LOAD_INIT\r\n");
	U_puts(buf);

	return true;
}

static bool SAVE_PARAM_func(){
	Params.csum = CSUM_calc((uint8_t*)&Params,sizeof(FLASH_ROM_Parameters)-CSUM_SIZE);
	writeFlash(DATA_ADDR,(uint8_t*)&Params,sizeof(FLASH_ROM_Parameters));

	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"SAVE_PARAM\r\n");
	U_puts(buf);

	return true;
}

static bool DUMP_PARAM_func(){
	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"DUMP_PARAM,");
	str_concat(buf,str_putx(Params.version ,8));str_concat(buf,",");
	str_concat(buf,str_putlf(Params.gain_p ,5));str_concat(buf,",");
	str_concat(buf,str_putlf(Params.gain_i ,5));str_concat(buf,",");
	str_concat(buf,str_putn2(Params.send_cycle_ms));str_concat(buf,",");
	str_concat(buf,str_putn2(Params.startup_time_s));str_concat(buf,"\r\n");
	U_puts(buf);

	return true;
}

static bool GET_VERSION_func(){
	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"GET_VERSION,");
	str_concat(buf,str_putx(Params.version,8));str_concat(buf,"\r\n");
	U_puts(buf);

	return true;
}

static bool SET_SEND_CYCLE_func(){
	if(len != 2)return false;

	if(!IsDecString(words[1]))return false;

	u16 cycle = DecStringToDec(words[1]);
	u16 min_cycle = (u16)(Get_CtrlCycle()*1000.0);

	cycle =cycle - cycle%min_cycle;

	if(cycle < min_cycle){
		return false;
	}

	Params.send_cycle_ms = cycle;
	Set_SendCycle((double)Params.send_cycle_ms/1000.0);

	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"SET_SEND_CYCLE,");
	str_concat(buf,str_putn2(Params.send_cycle_ms));
	str_concat(buf,"\r\n");
	U_puts(buf);

	return true;
}

static bool GET_PROD_ID_func(){
	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"GET_PROD_ID,ADIS");
	str_concat(buf,str_putn2(PRODUCT_ID));
#ifdef NONE_BMLZ
	str_concat(buf,"\r\n");
#else
	#if defined(_1BMLZ)
		str_concat(buf,"-1\r\n");
	#elif defined(_2BMLZ)
		str_concat(buf,"-2\r\n");
	#elif defined(_3BMLZ)
		str_concat(buf,"-3\r\n");
	#endif
#endif
	U_puts(buf);

	return true;
}

static bool GET_FORMAT_func(){
	char buf[256];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"GET_FORMAT,");

	u8 format = Get_Format();

	switch(format){
	case YawPitchRoll:
		str_concat(buf,"YAW[deg],PITCH[deg],ROLL[deg]");
		break;
	case GYRO_Degree:
		str_concat(buf,"X_GYRO[deg/s],Y_GYRO[deg/s],Z_GYRO[deg/s]");
		break;
	case GYRO_ACC_HexData:
		str_concat(buf,"X_GYRO_HEX,Y_GYRO_HEX,Z_GYRO_HEX,"
				"X_ACC_HEX,Y_ACC_HEX,Z_ACC_HEX,CSUM");
		break;
	case GYRO_ACC_BinaryData:
		return false;
		break;
	case YawPitchRoll_ACC:
		str_concat(buf,"YAW[deg],PITCH[deg],ROLL[deg],X_ACC[g],Y_ACC[g],Z_ACC[g]");
		break;
	case GYRO_ACC_TEMP:
		str_concat(buf,"X_GYRO[rad/s],Y_GYRO[rad/s],Z_GYRO[rad/s],X_ACC[g],Y_ACC[g],Z_ACC[g],TEMP[deg]");
		break;
	default:
		return false;
		break;
	}

	str_concat(buf,"\r\n");
	U_puts(buf);

	return true;
}

static bool GET_SENSI_func(){
	double gyro_sensi,acc_sensi;
	Get_SensSensi(&gyro_sensi,&acc_sensi);

	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"GET_SENSI,");
	str_concat(buf,str_putlf(gyro_sensi,1));str_concat(buf,",");
	str_concat(buf,str_putlf(acc_sensi,1));str_concat(buf,"\r\n");
	U_puts(buf);

	return true;
}

static bool GET_BOARD_NAME_func(){
	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"GET_BOARD_NAME,");
	str_concat(buf,BoardName);str_concat(buf,"\r\n");
	U_puts(buf);

	return true;
}

static bool GET_STATUS_func(){
	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"GET_STATUS,");

	if(IsAutoBiasUpdate()){
		str_concat(buf,"AutoBiasUpdating,");
		str_concat(buf,str_putn2(AutoBiasUpdate_TimeLeft()));
	}else{
		if(IsSendEnable()){
			str_concat(buf,"Running");
		}else{
			str_concat(buf,"Ready");
		}
	}

	str_concat(buf,"\r\n");
	U_puts(buf);

	return true;
}

static bool SET_STARTUP_TIME_func(){
	char buf[128];
	memset(buf,0,sizeof(buf));

	if(!IsDecString(words[1]))return false;

	u16 startup_time = DecStringToDec(words[1]);
	Params.startup_time_s = startup_time;

	str_concat(buf,"SET_STARTUP_TIME,");
	str_concat(buf,str_putn2(Params.startup_time_s));str_concat(buf,"\r\n");
	U_puts(buf);

	return true;
}

void other_func(){
	char buf[128];
	memset(buf,0,sizeof(buf));

	str_concat(buf,"ERROR_NONE_CMD,");
	str_concat(buf,words[0]);
	str_concat(buf,"\r\n");
	U_puts(buf);
}

void Params_initialize(){
	Params.version =VERSION;
	Params.gain_p = gain_p_init ;
	Params.gain_i = gain_i_init ;
	Params.send_cycle_ms =send_cycle_ms_init;
	Params.startup_time_s = startup_time_s_init;
	Params.csum = CSUM_calc((uint8_t*)&Params,sizeof(FLASH_ROM_Parameters)-CSUM_SIZE);

	Set_SendCycle((double)Params.send_cycle_ms/1000.0);
}
