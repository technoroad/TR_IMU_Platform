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

#ifndef INC_USB_UART_SELECT_H_
#define INC_USB_UART_SELECT_H_

void SelectUsbForCommunication(void);
void SelectUsartForCommunication(void);
bool IsSelectUsart(void);

bool COMM_IsTransmitFree();
void COMM_TransmitStart();
void COMM_ResetWriteBuffer();
void COMM_ResetDoubleBuffer();

bool COMM_PushDoubleBuffer(uint8_t *buf, uint32_t length);
void COMM_puts(char *buf);
void COMM_putd(char *buf, uint32_t length);

uint16_t COMM_GetDataCount();
char COMM_GetChar();
bool COMM_PollingSend(char *buf);

#endif /* INC_USB_UART_SELECT_H_ */
