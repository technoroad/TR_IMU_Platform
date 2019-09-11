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

#ifndef USART_CTRL_H_
#define USART_CTRL_H_

#include <libraries.h>

//void UART_init(u32 baud);
void UART_IT_ENABLE();
void UART_putn(s32 num,char len);
void UART_putx(u32 num,char len);
void UART_putf(float val,int bp,int ap);
void UART_putlf(float val,int ap);

extern void UART_putc(char data);
extern void UART_puts(char* buf);
extern void UART_putd(char* buf,u16 len);
extern signed char UART_getc();

void UART_init_ring_buf(void);
u16 UART_get_cap(void);
char UART_get_ring_buf(void);
void UART_put_ring_buf(char c);

#endif /* USART_CTRL */
