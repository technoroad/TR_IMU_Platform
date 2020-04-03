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

#define CMDBUF_LEN 64
static volatile char CMDBUF[CMDBUF_LEN];

// SPI to USB mode
// Compatible with ADI DRIVER
void BinaryCmdParse(char* cmd,u32 len)
{
	// Exit if there are not enough received characters
	if(len < 3)cmd[0] =0;

	if(cmd[0] == 0x5A){ // Initial setting command
		//Do not change settings, but only reply
		char buf[2] ={0xFF,0x00};
		U_putd(buf,2);

	}else if(cmd[0] == 0x61){ // Data manipulation command
#if defined(ADIS16495)
		if(cmd[1] == PAGE0_BURST_CMD){   //Burst mode
			u16 rb[18];
			memset(rb,0,sizeof(rb));

			// Read the sensor value
			ADIS_32bit_READ(rb);

			char sb[128];
			memset(sb,0,sizeof(sb));

			// Creating transmission packet
			sb[0] = 0xff;
			for(int i =0;i<18;i++){
				sb[i*2+3] =rb[i]>>8 &0xff;
				sb[i*2+4] =rb[i] &0xff;
			}

			//Because of the special format of USB-ISS,
			//it is sent at 10 character delimiter at the time of burst read.
			if(len ==38){
				U_putd(sb,40);
			}else{
				U_putd(sb,len);
			}
#else
		if(cmd[1] == BURST_CMD){   //Burst mode
			u16 rb[10];
			memset(rb,0,sizeof(rb));

			// Read the sensor value
			ADIS_NoBloking_RegRead(BURST_CMD);
			for(int i =0;i<10;i++){
				rb[i]=ADIS_NoBloking_RegRead(0);
			}

			char sb[128];
			memset(sb,0,sizeof(sb));

			// Creating transmission packet
			sb[0] = 0xff;
			for(int i =0;i<10;i++){
				sb[i*2+3] =rb[i]>>8 &0xff;
				sb[i*2+4] =rb[i] &0xff;
			}

			//Because of the special format of USB-ISS,
			//it is sent at 10 character delimiter at the time of burst read.
			if(len ==24){
				U_putd(sb,30);
			}else{
				U_putd(sb,len);
			}
#endif
		}else if((cmd[1]&0x80) == 0){	// Read command

			//Because the received character is always odd,
			//forced termination occurs even
			if(len %2==0)return;

			// Read the sensor value
			u16 rb[128];
			memset(rb,0,sizeof(rb));

			int j = 0;
			for(int i=1;i<len;i+=2){
				rb[j++] =ADIS_NoBloking_RegRead(cmd[i]);
			}


			char sb[128];
			memset(sb,0,sizeof(sb));

			// Creating transmission packet
			sb[0]=0xff;
			for(int i=0;i<((len-1)/2);i++){
				sb[(i*2)+1] =rb[i]>>8 &0xff;
				sb[(i*2)+2] =rb[i]&0xff;
			}

			// Transmission
			U_putd(sb,len);

		}else if((cmd[1]&0x80) != 0){	// Write command

			//Because the received character is always odd,
			//forced termination occurs even
			if(len %2==0)return;

			for(int i=1;i<len;i+=2){
				ADIS_RegWrite_8bit(cmd[i],cmd[i+1]);
			}

			char sb[128];
			memset(sb,0,sizeof(sb));

			// Transmission
			sb[0]=0xff;
			U_putd(sb,len);
		}
	}

	// For processing time confirmation
	DEBUG_PIN1_LOW;
}

#define TIME_CNT_LIM 0x200

void ReadBinaryCmd(){
	static vu32 time_cnt = 0;
	static vu32 RECVBUF_LEN = 0;

	if(IsEnable_UART()){		//Separate processing with UART and USB.
		// UART processing
		if(U_get_cap()!=0){		//Evaluate the number of characters received
			// For processing time confirmation
			DEBUG_PIN1_HIGH;

			// Store received characters in buffer
			LD1_ON;
			char rb= U_getc();
			CMDBUF[RECVBUF_LEN++] =rb;
			time_cnt =0;
			LD1_OFF;

		}else{
			// Add counter for timeout
			if(RECVBUF_LEN >0){
				time_cnt++;
			}
		}

		// Process the command if a timeout occurs.
		if(time_cnt >TIME_CNT_LIM){
			// Command parse
			BinaryCmdParse((char*)CMDBUF,RECVBUF_LEN);

			RECVBUF_LEN =0;
			time_cnt=0;
			memset((char*)CMDBUF,0,sizeof(CMDBUF));
		}
	}else{
		//USB processing
		if(USB_get_cap()!=0){
			// For processing time confirmation
			DEBUG_PIN1_HIGH;

			// Store received packets in command buffer
			u8 pl =USB_get_pl_ring_buf();
			for(int i=0;i<pl;i++){
				char rb= USB_getc();
				CMDBUF[RECVBUF_LEN++] =rb;
			}

			// Command parse
			BinaryCmdParse((char*)CMDBUF,RECVBUF_LEN);

			RECVBUF_LEN =0;
			memset((char*)CMDBUF,0,sizeof(CMDBUF));
		}
	}
}
