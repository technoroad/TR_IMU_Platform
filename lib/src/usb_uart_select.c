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

u8 UART_FLAG =0;

void USB_Select(void){
	UART_FLAG =0;
}

void UART_Select(void){
	UART_FLAG =1;
}

u8 IsEnable_UART(void){
	return UART_FLAG;
}

void U_puts(char * s){
	if(UART_FLAG){
		//UART
		UART_puts(s);
	}else{
		//USB
		USB_puts(s);
	}
}

void U_putd(char * s,char len){
	if(UART_FLAG){
		//UART
		UART_putd(s,len);
	}else{
		//USB
		USB_putd(s,len);
	}
}

void U_putx(u32 num,char len){
	if(UART_FLAG){
		//UART
		UART_putx(num,len);
	}else{
		//USB
		USB_putx(num,len);
	}
}

void U_putn(s32 num,char len){
	if(UART_FLAG){
		//UART
		UART_putn(num,len);
	}else{
		//USB
		USB_putn(num,len);
	}
}

void U_putf(float val,char beforepoint,char afterpoint){
	if(UART_FLAG){
		//UART
		UART_putf(val,beforepoint,afterpoint);
	}else{
		//USB
		USB_putf(val,beforepoint,afterpoint);
	}
}

void U_putlf(float val,char afterpoint){
	if(UART_FLAG){
		//UART
		UART_putlf(val,afterpoint);
	}else{
		//USB
		USB_putlf(val,afterpoint);
	}
}

u16 U_get_cap(){
	if(UART_FLAG){
		//UART
		return UART_get_cap();
	}else{
		//USB
		return USB_get_cap();
	}
}

char U_getc(){
	if(UART_FLAG){
		//UART
		return UART_getc();
	}else{
		//USB
		return USB_getc();
	}
}
