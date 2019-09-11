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

/* Includes ------------------------------------------------------------------*/
#include "usbprint.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
//==============================================================================
// Convert integer value to hexadecimal string and transfer.
//==============================================================================
void USB_putx(u32 num,u8 len){
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
	USB_putd((char*)s,n);
}

//==============================================================================
// Convert integer value to string and transmission.
//==============================================================================
void USB_putn(s32 num,u8 len){
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
	USB_putd((char*)s,n);
}

#define USB_TIME_OUT 1000
void USB_BLOCKING(u32 length){
	u32 i=0;
	u32 time_out = USB_TIME_OUT *length;

	while(((USBD_CDC_HandleTypeDef*)(hUsbDeviceFS.pClassData))->TxState!=0){
		delay_us(1);
		i++;
		if(i>time_out){
			break;
		}
	}
}

static char sb[256];

//==============================================================================
// One character output with USB
//==============================================================================
void USB_putc(char data){
	sb[0] = data;

	CDC_Transmit_FS((u8 *)&sb,1);
//	USB_BLOCKING(1);
}

//==============================================================================
// String output with USB
//==============================================================================
void USB_puts(char* buf){
	memset(sb,0,sizeof(sb));

	for(int i=0;i<strlen(buf);i++){
		sb[i] = buf[i];
	}

	CDC_Transmit_FS((u8 *)sb,strlen(buf));
//	USB_BLOCKING(1);
}

//==============================================================================
// Character string output specified with USB (string, length of characters)
//==============================================================================
void USB_putd(char* buf,u8 len){
	memset(sb,0,sizeof(sb));

	for(int i=0;i<len;i++){
		sb[i] = buf[i];
	}

	CDC_Transmit_FS((u8 *)sb,len);
//	USB_BLOCKING(len);
}

//==============================================================================
// Get 1 character from ring buffer.
//==============================================================================
u8 USB_getc(){
	if(USB_get_cap() == 0){
		return 0;
	}
	return USB_get_ring_buf();
}

//==============================================================================
// Convert float to string and transfer.
//==============================================================================
void USB_putf(float val,int bp,int ap){
	USB_puts(str_putf(val,bp,ap));
}

//==============================================================================
// Convert double to string and transfer.
//==============================================================================
void USB_putlf(float val,int ap){
	USB_puts(str_putlf(val,ap));
}
