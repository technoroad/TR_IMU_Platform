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

/* Includes ------------------------------------------------------------------*/
#include "libraries.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define SPI_BUF_SIZE 32

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static volatile uint16_t kSendBuf[SPI_BUF_SIZE] = { 0 };
static volatile uint16_t kReadBuf[SPI_BUF_SIZE] = { 0 };

/* Private function prototypes -----------------------------------------------*/
uint32_t AddWriteCurrentPage(volatile uint16_t *buf, uint8_t page);

/* Global variables ----------------------------------------------------------*/

/**
 * @brief  Write to any address
 * @note   The page works only for the ADIS1649X series.
 */
void IMU_WriteData(uint8_t page, uint8_t addr, uint16_t data) {
  int len = 0;
#if defined(ADIS1647X)
  kSendBuf[len++] = ((addr | 0x80) << 8) | (data & 0xFF);
  kSendBuf[len++] = ((addr | 0x81) << 8) | ((data >> 8) & 0xFF);

  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) kReadBuf, len);
#elif defined(ADIS1649X)
  len += AddWriteCurrentPage(&kSendBuf[len],page);
  kSendBuf[len++] = ((addr | 0x80) << 8) | (data & 0xFF);
  kSendBuf[len++] = ((addr | 0x81) << 8) | ((data >> 8) & 0xFF);
  len += AddWriteCurrentPage(&kSendBuf[len],PAGE0);

  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) kReadBuf, len);
#endif
}

/**
 * @brief  Read any address
 * @note   The page works only for the ADIS1649X series.
 */
uint16_t IMU_ReadData(uint8_t page, uint16_t addr) {
  // Burst command can not be read
  if (IMU_IsBurstReadCommand(page, addr)) {
    return 0;
  }

  int len = 0;
#if defined(ADIS1647X)
  kSendBuf[len++] = (addr << 8) & 0xFF00;
  kSendBuf[len++] = 0x00;

  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) kReadBuf, len);
  return kReadBuf[1];
#elif defined(ADIS1649X)
  len += AddWriteCurrentPage(&kSendBuf[len],page);
  kSendBuf[len++] = (addr << 8) & 0xFF00;
  kSendBuf[len++] = 0x00;
  len += AddWriteCurrentPage(&kSendBuf[len],PAGE0);

  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) kReadBuf, len);
  return kReadBuf[3];
#endif
}

/**
 * @brief  Change the current page number.
 */
uint32_t AddWriteCurrentPage(volatile uint16_t *buf, uint8_t page) {
#if defined(ADIS1649X)
  // page write
  buf[0] = (0x80 << 8) | (page & 0xFF);
  buf[1] = (0x81 << 8) | ((page >> 8) & 0xFF);
  return 2;
#endif
  return 0;
}

/**
 * @brief  Read all register
 */
uint16_t IMU_ReadAllPageData(uint8_t page, uint16_t *data) {
  int len = 0;
  len += AddWriteCurrentPage(&kSendBuf[len], page);
  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) data, len);

  len = 0;
  for (int i = 0; i <= 0x7E; i += 2) {
    if (IMU_IsBurstReadCommand(page, i)) {
      kSendBuf[len++] = 0x00;
    } else {
      kSendBuf[len++] = (i << 8) & 0xFF00;
    }
  }
  kSendBuf[len++] = 0x0000;
  uint16_t ret = len;
  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) data, len);

  len = 0;
  len += AddWriteCurrentPage(&kSendBuf[len], PAGE0);
  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) data, len);
  return ret;
}

/**
 * @brief  Updating gyro bias.
 */
void IMU_UpdateGyroBias() {
  int len = 0;
  len += AddWriteCurrentPage(&kSendBuf[len], PAGE2);
  for (int i = XG_BIAS_LOW; i <= ZG_BIAS_HIGH; i += 2) {
    kSendBuf[len++] = (i << 8) & 0xFF00;
  }
  kSendBuf[len++] = (DATA_CNT << 8) & 0xFF00;
  kSendBuf[len++] = 0x0000;
  len += AddWriteCurrentPage(&kSendBuf[len], PAGE0);

  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) kReadBuf, len);
  IMU_ConvertBiasData((uint16_t*) &kReadBuf[1]);
}

/**
 * @brief  Updating 6-axis data with a burst read
 */
void IMU_Update6AxisDataWithBurstRead() {
  // Set all data in the transmit buffer to 0.
  for (int i = 0; i < SPI_BUF_SIZE; i++) {
    kSendBuf[i] = 0;
  }

  kSendBuf[0] = BURST_CMD << 8;
  SPI_SetBaudRate(SPI_BURST_PRES);
  SPI_WriteReceiveBurstRead((uint16_t*) kSendBuf, (uint16_t*) kReadBuf,
                            BURST_CMD_SIZE);
  SPI_SetBaudRate(SPI_SINGLE_PRES);

  DelayMicrosecond(TSTALL);

  IMU_Convert32bitDataWithBurstRead((uint16_t*) &kReadBuf[1]);
}

#if defined(ADIS1647X)
/**
 * @brief  Updating 6-axis data with a single read.
 */
void IMU_Update6AxisDataWithSingleRead() {
  int len = 0;
  for (int i = DIAG_STAT; i <= TEMP_OUT; i += 2) {
    kSendBuf[len++] = (i << 8) & 0xFF00;
  }
  kSendBuf[len++] = (DATA_CNT << 8) & 0xFF00;
  kSendBuf[len++] = 0x0000;

  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) kReadBuf, len);
  IMU_Convert32bitDataWithSingleRead((uint16_t*) &kReadBuf[1]);
}
#elif defined(ADIS1649X)
/**
 * @brief  Updating 6-axis data with a single read.
 */
void IMU_Update6AxisDataWithSingleRead() {
  int len = 0;
  kSendBuf[len++] = (SYS_E_FLAG << 8) & 0xFF00;
  for (int i = TEMP_OUT; i <= Z_ACCL_OUT; i += 2) {
    kSendBuf[len++] = (i << 8) & 0xFF00;
  }
  kSendBuf[len++] = (DATA_CNT << 8) & 0xFF00;
  kSendBuf[len++] = 0x0000;

  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) kReadBuf, len);
  IMU_Convert32bitDataWithSingleRead((uint16_t*) &kReadBuf[1]);
}

#endif
