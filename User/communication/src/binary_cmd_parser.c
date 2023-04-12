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
#define BIN_BUF_SIZE 128
#define MSG_BUF_SIZE 128
#define SPI_BUF_SIZE 128
#define TIME_CNT_LIM 0x200

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static char kBinaryBuf[BIN_BUF_SIZE] = { 0 };
static char kMsgBuf[MSG_BUF_SIZE] = { 0 };
static uint16_t kSpiBuf[SPI_BUF_SIZE] = { 0 };
static bool kEnableAdiDriver = false;

/* Private function prototypes -----------------------------------------------*/
uint16_t SPI_Read1byte(uint8_t addr);
void SPI_Write1byte(uint8_t regAddr, uint8_t regData);

/* Global variables ----------------------------------------------------------*/

/**
 * @brief  This function performs bi-directional conversion between USB and SPI.
 * @details  Simulates the operation when
 *           the USB-ISS and 1647X series are directly connected.
 */
void BinaryCmdParse(char *cmd, uint32_t len) {
  // Exit if there are not enough received characters
  if (len < 3) {
    cmd[0] = 0;
  }

  if (cmd[0] == 0x5A) {  // Initial setting command
    // Do not change settings, but only reply
    char buf[2] = { 0xFF, 0x00 };
    COMM_putd(buf, 2);

  } else if (cmd[0] == 0x61) {  // Data manipulation command
    if (cmd[1] == BURST_CMD) {  // Burst mode
      memset(kSpiBuf, 0, sizeof(kSpiBuf));

      // Read the sensor value
      SPI_Read1byte(BURST_CMD);
      for (int i = 0; i < 10; i++) {
        kSpiBuf[i] = SPI_Read1byte(0);
      }

      memset(kMsgBuf, 0, sizeof(kMsgBuf));

      // Creating transmission packet
      kMsgBuf[0] = 0xff;
      for (int i = 0; i < 10; i++) {
        kMsgBuf[i * 2 + 3] = kSpiBuf[i] >> 8 & 0xff;
        kMsgBuf[i * 2 + 4] = kSpiBuf[i] & 0xff;
      }

      //Because of the special format of USB-ISS,
      //it is sent at 10 character delimiter at the time of burst read.
      if (len == 24) {
        COMM_putd(kMsgBuf, 30);
      } else {
        COMM_putd(kMsgBuf, len);
      }
    } else if ((cmd[1] & 0x80) == 0) {  // Read command

      //Because the received character is always odd,
      //forced termination occurs even
      if (len % 2 == 0) {
        return;
      }

      // Read the sensor value
      memset(kSpiBuf, 0, sizeof(kSpiBuf));

      int j = 0;
      for (int i = 1; i < len; i += 2) {
        kSpiBuf[j++] = SPI_Read1byte(cmd[i]);
      }

      memset(kMsgBuf, 0, sizeof(kMsgBuf));

      // Creating transmission packet
      kMsgBuf[0] = 0xff;
      for (int i = 0; i < ((len - 1) / 2); i++) {
        kMsgBuf[(i * 2) + 1] = kSpiBuf[i] >> 8 & 0xff;
        kMsgBuf[(i * 2) + 2] = kSpiBuf[i] & 0xff;
      }

      // Transmission
      COMM_putd(kMsgBuf, len);

    } else if ((cmd[1] & 0x80) != 0) {  // Write command

      //Because the received character is always odd,
      //forced termination occurs even
      if (len % 2 == 0) {
        return;
      }

      for (int i = 1; i < len; i += 2) {
        SPI_Write1byte(cmd[i], cmd[i + 1]);
      }

      memset(kMsgBuf, 0, sizeof(kMsgBuf));

      // Transmission
      kMsgBuf[0] = 0xff;
      COMM_putd(kMsgBuf, len);
    }
  }
}

/**
 * @brief  Get data from USB or USART and
 *         set packet length and command to BinaryCmdParse().
 * @note  USB is determined by packet length.
 *        USART is determined by timeout.
 */
void ReadBinaryCmd() {
  static uint32_t time_cnt = 0;
  static uint32_t counter = 0;

  if (IsSelectUsart()) {  // Separate processing with UART and USB.
    // UART processing
    if (COMM_GetDataCount() != 0) {  //Evaluate the number of characters received

      // Store received characters in buffer
      char c = COMM_GetChar();
      kBinaryBuf[counter++] = c;
      time_cnt = 0;

    } else {
      // Add counter for timeout
      if (counter > 0) {
        time_cnt++;
      }
    }

    // Process the command if a timeout occurs.
    if (time_cnt > TIME_CNT_LIM) {
      // Command parse
      BinaryCmdParse((char*) kBinaryBuf, counter);

      counter = 0;
      time_cnt = 0;
      memset((char*) kBinaryBuf, 0, sizeof(kBinaryBuf));
    }
  } else {
    //USB processing
    if (USB_GetDataCount() != 0) {

      // Store received packets in command buffer
      uint8_t pl = USB_PullPacketLength();
      for (int i = 0; i < pl; i++) {
        char c = USB_GetChar();
        kBinaryBuf[counter++] = c;
      }

      // Command parse
      BinaryCmdParse(kBinaryBuf, counter);

      counter = 0;
      memset(kBinaryBuf, 0, sizeof(kBinaryBuf));
    }
  }
}

/**
 * @brief  ADI driver is enabled.
 */
void EnableAdiDriver() {
  kEnableAdiDriver = true;
}

/**
 * @brief  ADI driver is disabled.
 */
void DisableAdiDriver() {
  kEnableAdiDriver = false;
}

/**
 * @brief  Returns whether ADI driver mode is enabled.
 */
bool IsEnableAdiDriver() {
  return kEnableAdiDriver;
}

/**
 * @brief  SPI read without blocking
 */
uint16_t SPI_Read1byte(uint8_t addr) {
  uint16_t send = (addr << 8) & 0xFF00;
  uint16_t read = 0;

  SPI_WriteReceive((uint16_t*) &send, (uint16_t*) &read, 1);
  return read;
}

/**
 * @brief  Write 1byte data to register
 */
void SPI_Write1byte(uint8_t addr, uint8_t data) {
  uint16_t send = ((addr | 0x80) << 8) | (data & 0xFF);
  uint16_t read = 0;

  SPI_WriteReceive((uint16_t*) &send, (uint16_t*) &read, 1);
}
