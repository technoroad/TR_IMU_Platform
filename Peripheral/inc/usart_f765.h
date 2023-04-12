/*
 The MIT License (MIT)

 Copyright (c) 2023 Techno Road Inc.

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

#ifndef PERIPHERAL_INC_USART_F765_H_
#define PERIPHERAL_INC_USART_F765_H_

#if defined(STM32F765xx)
void USART_Handler();
void USART_TransmitDmaHandler();
void USART_ReceiveDmaHandler();

void USART_TransmitDmaEnable();
void USART_ReceiveDmaEnable();

bool USART_PushWriteBuffer(uint8_t *buf, uint32_t length);
void USART_TransmitStart();
void USART_AnyAddressTransmit(char *pointer, uint32_t length);
void USART_ChangeBaudRate(uint32_t baud);
void USART_BusyTimeout();
void USART_TransmitBufferReset();

bool USART_Puts(char *buf);
bool USART_Putd(char *buf, uint32_t length);
void USART_Putx(uint32_t num, uint8_t len);
void USART_Putn(int32_t num, uint8_t len);
void USART_Putf(float val, int bp, int ap);
void USART_Putlf(float val, int ap);
char USART_GetChar(void);
bool USART_IsTransmitFree();

uint32_t USART_GetNewDataIndex();
uint32_t USART_GetDataCount(void);
char USART_GetRingBuf(void);

void USART_Abort();
uint8_t USART_ErrorCheck();

void USART_BusyTimeout();
#endif

#endif /* PERIPHERAL_INC_USART_F765_H_ */
