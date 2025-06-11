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

#ifndef PERIPHERAL_INC_SPI_F765_H_
#define PERIPHERAL_INC_SPI_F765_H_

#if defined(STM32F765xx)
void SPI_SetPrescaler(uint32_t ll_baud);
void SPI_SetTStall(uint8_t t);
void SPI_ReceiveDmaHandler(void);
void SPI_WriteReceive(uint16_t *wb, uint16_t *rb, uint16_t length);
void SPI_WriteReceiveBurstRead(uint16_t *wb, uint16_t *rb, uint16_t length);
#endif

#endif /* PERIPHERAL_INC_SPI_F765_H_ */
