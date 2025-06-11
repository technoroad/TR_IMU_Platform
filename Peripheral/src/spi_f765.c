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
#if defined(STM32F765xx)

/* Includes ------------------------------------------------------------------*/
#include "libraries.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
// DMA
#define DMAx DMA2
// SPI
#define SPIx SPI4
#define TIMEOUT (1000-1)  // us
// Receive
#define RX_Stream LL_DMA_STREAM_0
#define LL_DMA_IsActiveFlag_RX LL_DMA_IsActiveFlag_TC0
#define LL_DMA_ClearFlag_RX LL_DMA_ClearFlag_TC0
#define LL_DMA_NDTR DMA2_Stream0->NDTR

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static uint32_t kTimeoutCnt = 0;
static uint8_t kTStall = 16;

/* Private function prototypes -----------------------------------------------*/
static bool SPI_TimeoutReporter();
void SPI_ReceiveDmaEnable(uint16_t *rb,uint16_t len);

/* Global variables ----------------------------------------------------------*/

/**
 * @brief  Sets the SPI speed.
 */
void SPI_SetPrescaler(uint32_t ll_baud) {
  LL_SPI_SetBaudRatePrescaler(SPIx, ll_baud);
}

void SPI_SetTStall(uint8_t t){
  kTStall = t;
}

/***
 * @brief  DMA interrupt function for USART receive complete
 */
void SPI_ReceiveDmaHandler(void) {
  // Receive complete interrupt
  if (LL_DMA_IsActiveFlag_RX(DMAx) == 1) {
    // Reset receive complete interrupt
    LL_DMA_ClearFlag_RX(DMAx);
  }
}

/**
 *
 */
void SPI_ReceiveDmaEnable(uint16_t *rb,uint16_t len) {
  // Disable SPI
  LL_SPI_Disable(SPIx);
  // Disable DMA channel
  LL_DMA_DisableStream(DMAx, RX_Stream);
  // Configure DMA address
  LL_DMA_ConfigAddresses(DMAx, RX_Stream,  //
                         LL_SPI_DMA_GetRegAddr(SPIx), (uint32_t) rb,
                         LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  // Set data length address
  LL_DMA_SetDataLength(DMAx, RX_Stream, len);
  // Enable DMA channel
  LL_DMA_EnableIT_TC(DMAx, RX_Stream);
  LL_DMA_EnableStream(DMAx, RX_Stream);
  LL_SPI_EnableDMAReq_RX(SPIx);
  LL_SPI_Enable(SPIx);
}

/**
 * @brief  Write and read at the same time with SPI.
 */
void SPI_WriteReceive(uint16_t *wb, uint16_t *rb, uint16_t length) {
  SPI_ReceiveDmaEnable(rb,length);

  for (int i = 0; i < length; i++) {
    SPI_ENABLE_CHIPSELECT;
    LL_SPI_TransmitData16(SPIx, wb[i]);
    kTimeoutCnt = 0;
    while (LL_SPI_IsActiveFlag_BSY(SPIx)) {
      if (SPI_TimeoutReporter()) {
        SetSystemError(kSpiTimeoutError);
        break;
      }
    }
    SPI_DISABLE_CHIPSELECT;

    DelayMicrosecond(kTStall);
  }
}

/**
 * @brief  Write and read at the same time with SPI.
 */
void SPI_WriteReceiveBurstRead(uint16_t *wb, uint16_t *rb, uint16_t length) {
  SPI_ReceiveDmaEnable(rb,length);

  SPI_ENABLE_CHIPSELECT;
  for (int i = 0; i < length; i++) {
    LL_SPI_TransmitData16(SPIx, wb[i]);
    kTimeoutCnt = 0;
    while (LL_SPI_IsActiveFlag_BSY(SPIx)) {
      if (SPI_TimeoutReporter()) {
        SetSystemError(kSpiTimeoutError);
        break;
      }
    }
  }
  SPI_DISABLE_CHIPSELECT;
  DelayMicrosecond(kTStall);
}

/**
 * @brief  Counts the SPI wait time and returns the timeout flag.
 */
bool SPI_TimeoutReporter() {
  if (kTimeoutCnt > TIMEOUT) {
    kTimeoutCnt = 0;
    return true;
  } else {
    DelayMicrosecond(1);
    kTimeoutCnt++;
    return false;
  }
}

#endif
