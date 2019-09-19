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

#include "USART_CTRL.h"

#define USART4_BUFFER_SIZE 1024
static char ring_buf1[USART4_BUFFER_SIZE];	// ring buffer size
static vu16 wp = 0;							// write pointer
static vu16 rp = 0;							// read pointer

extern UART_HandleTypeDef huart4;
UART_HandleTypeDef * phuart =&huart4;
static USART_TypeDef * USART =UART4;

uint8_t RxData = 0;

//Receive interrupt
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle->Instance==USART){
		UART_put_ring_buf(RxData);
		RxData =0;
		HAL_UART_Receive_IT(phuart, &RxData, 1);
	}
}

//Transmit interrupt
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle->Instance==USART){
		// For checking USB and UART communication time
		DEBUG_PIN1_LOW;
	}
}

//function to receive interrupt enable
void UART_IT_ENABLE(){
	  HAL_UART_Receive_IT(phuart, &RxData, 1);
}

static char sb[256];

//==============================================================================
// One character output with UART
//==============================================================================
void UART_putc(char data){
	sb[0] = data;

	HAL_UART_Transmit_IT(phuart,(uint8_t*)&sb,1);
}

//==============================================================================
// String output with UART
//==============================================================================
void UART_puts(char* buf){
	memset(sb,0,sizeof(sb));

	for(int i=0;i<strlen(buf);i++){
		sb[i] = buf[i];
	}

	HAL_UART_Transmit_IT(phuart,(uint8_t*)sb,strlen(buf));
}

//==============================================================================
// Character string output specified with UART (string, length of characters)
//==============================================================================
void UART_putd(char* buf,u16 len){
	memset(sb,0,sizeof(sb));

	for(int i=0;i<len;i++){
		sb[i] = buf[i];
	}

	HAL_UART_Transmit_IT(phuart,(uint8_t*)sb,len);
}

//==============================================================================
// Convert integer value to string and transmission.
//==============================================================================
void UART_putn(s32 num,char len){
	u8 s[10],n=1;
	if(num<0){s[0]='-';num=-num;}
	else{s[0]=' ';}
	switch(len){
		case  9:s[n]=(u8)('0'+((num%1000000000)/100000000));n++;
		case  8:s[n]=(u8)('0'+((num%100000000)/10000000));n++;
		case  7:s[n]=(u8)('0'+((num%10000000)/1000000));n++;
		case  6:s[n]=(u8)('0'+((num%1000000)/100000));n++;
		case  5:s[n]=(u8)('0'+((num%100000)/10000));n++;
		case  4:s[n]=(u8)('0'+((num%10000)/1000));n++;
		case  3:s[n]=(u8)('0'+((num%1000)/100));n++;
		case  2:s[n]=(u8)('0'+((num%100)/10));n++;
		case  1:s[n]=(u8)('0'+((num%10)/1));n++;
	}
	UART_putd((char *)s,n);
}

//==============================================================================
// Convert integer value to hexadecimal string and transfer.
//==============================================================================
void UART_putx(u32 num,char len){
	u8 s[12],n=2;
	s[0]='0';
	s[1]='x';
	switch(len){
		case  8:s[n]=put_x16( num/0x10000000);n++;
		case  7:s[n]=put_x16((num%0x10000000)/0x1000000);n++;
		case  6:s[n]=put_x16((num%0x1000000)/0x100000);n++;
		case  5:s[n]=put_x16((num%0x100000)/0x10000);n++;
		case  4:s[n]=put_x16((num%0x10000)/0x1000);n++;
		case  3:s[n]=put_x16((num%0x1000)/0x100);n++;
		case  2:s[n]=put_x16((num%0x100)/0x10);n++;
		case  1:s[n]=put_x16((num%0x10));n++;
	}
	UART_putd((char*)s,n);
}

//==============================================================================
// Convert float to string and transfer.
//==============================================================================
void UART_putf(float val,int bp,int ap){
	UART_puts(str_putf(val,bp,ap));
}

//==============================================================================
// Convert double to string and transfer.
//==============================================================================
void UART_putlf(float val,int ap){
	UART_puts(str_putlf(val,ap));
}

//==============================================================================
// Get 1 character from ring buffer.
//==============================================================================
signed char UART_getc(){
	if(UART_get_cap() == 0){
		return 0;
	}
	return UART_get_ring_buf();
}

//==============================================================================
// Initialize the ring buffer.
//==============================================================================
void UART_init_ring_buf(void){
	memset(ring_buf1,0,sizeof(ring_buf1));
	wp = 0;
	rp = 0;
}

//==============================================================================
// Get length of data in ring buffer
//==============================================================================
u16 UART_get_cap(void){
	if(wp >= rp){
		return (wp - rp);
	}
	return (u16)(((s32)65535 + 1) + ((s32)wp - (s32)rp));
}

//==============================================================================
// Read from ring buffer
//==============================================================================
char UART_get_ring_buf(void){
	return ring_buf1[rp++ %USART4_BUFFER_SIZE];
}

//==============================================================================
// Write to ring buffer
//==============================================================================
void UART_put_ring_buf(char c){
	ring_buf1[wp++ % USART4_BUFFER_SIZE] = c;
}
