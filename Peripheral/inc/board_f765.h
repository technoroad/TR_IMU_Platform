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

#ifndef PERIPHERAL_F765_INC_BOARD_H_
#define PERIPHERAL_F765_INC_BOARD_H_

#if defined(STM32F765xx)
#define TR_IMU_PLATFORM
#define BoardName "TR-IMU-Platform"
#define CTRL_PERIOD 0.001  // [s]
#define SEND_BUFFER_SIZE 1024

// SPI baudrate chage by sensor.
#if defined(ADIS1647X)
#define SPI_SINGLE_PRES LL_SPI_BAUDRATEPRESCALER_DIV64
#define SPI_BURST_PRES LL_SPI_BAUDRATEPRESCALER_DIV128
#elif defined(ADIS1649X)
#define SPI_SINGLE_PRES LL_SPI_BAUDRATEPRESCALER_DIV32
#define SPI_BURST_PRES LL_SPI_BAUDRATEPRESCALER_DIV32
#else
#error "SPI baudrate definition error"
#endif

// Address to start writing data to flash memory
#define DATA_ADDR 0x080C0000

#define CSW1_MASK 0x01
#define CSW2_MASK 0x02
#define CSW3_MASK 0x04
#define CSW4_MASK 0x08
#define CSW5_MASK 0x10
#define CSW6_MASK 0x20
#define CSW7_MASK 0x40

void CheckConfigrationSwitch();
void ReadParameterFromFlash();
#endif

#endif /* PERIPHERAL_F765_INC_BOARD_H_ */
