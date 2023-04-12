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
#define DOUBLE_BUFFER_SIZE SEND_BUFFER_SIZE
#define TIMEOUT (1000000-1)  // us

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static bool kUsartFlag = 0;
static uint8_t kDoubleBuf[DOUBLE_BUFFER_SIZE] = { 0 };
static int32_t kBufferIndex = 0;
static uint32_t kTimeoutCnt = 0;

/* Private function prototypes -----------------------------------------------*/
static bool PollingTimeoutReporter();
/* Global variables ----------------------------------------------------------*/

/**
 * @brief  Communication uses USB.
 */
void SelectUsbForCommunication(void) {
  kUsartFlag = false;
}

/**
 * @brief  Communication uses USART.
 */
void SelectUsartForCommunication(void) {
  kUsartFlag = true;
}

/**
 * @brief  Returns true when setting to use USART for communication.
 *         Returns false when USB.
 */
bool IsSelectUsart(void) {
  return kUsartFlag;
}

/**
 * @brief  Returns true when sending is free.
 */
bool COMM_IsTransmitFree() {
  if (kUsartFlag) {
    // USART
    return USART_IsTransmitFree();
  } else {
    // USB
    return USB_IsTransmitFree();
  }
}

/**
 * @brief  Set kDoubleBuf to USART or USB transmit buffer
 *         and start transmitting.
 */
void COMM_TransmitStart() {
  // Send only when there is data
  if (kBufferIndex > 0) {
    if (kUsartFlag) {
      if (USART_ErrorCheck()) {
        SetSystemError(kUsartHardError);
      } else {
        // USART
        USART_BusyTimeout();
        USART_TransmitBufferReset();
        USART_PushWriteBuffer(kDoubleBuf, kBufferIndex);
        USART_TransmitStart();
      }
    } else {
      // USB
      USB_TransmitBufferReset();
      USB_PushWriteBuffer(kDoubleBuf, kBufferIndex);
      USB_TransmitStart();
    }

    // Reset double buffer
    kBufferIndex = 0;
  }
}

/**
 * @brief  Reset the transmit buffer.
 */
void COMM_ResetWriteBuffer() {
  if (kUsartFlag) {
    // USART
    USART_TransmitBufferReset();
  } else {
    // USB
    USB_TransmitBufferReset();
  }
}

/**
 * @brief  Reset the double buffer.
 */
void COMM_ResetDoubleBuffer() {
  memset(kDoubleBuf, 0, sizeof(kDoubleBuf));
  kBufferIndex = 0;
}

/**
 * @brief  Set data in the double buffer.
 */
bool COMM_PushDoubleBuffer(uint8_t *buf, uint32_t length) {
  // Data length check
  if ((kBufferIndex + length) > DOUBLE_BUFFER_SIZE) {
    return false;
  }

  // push data in write buffer
  for (int i = 0; i < length; i++) {
    kDoubleBuf[kBufferIndex + i] = buf[i];
  }
  kBufferIndex += length;

  return true;
}

/**
 * @brief  Sets a string in a double buffer.
 */
void COMM_puts(char *buf) {
  uint32_t length = strlen(buf);
  if (!COMM_PushDoubleBuffer((uint8_t*) buf, length)) {
    SetSystemError(kTransmitBufferOverflow);
  }
}

/**
 * @brief  Set only the data length in the double buffer.
 */
void COMM_putd(char *buf, uint32_t length) {
  if (!COMM_PushDoubleBuffer((uint8_t*) buf, length)) {
    SetSystemError(kTransmitBufferOverflow);
  }
}

/**
 * @brief  Returns the data length received by USB or USART.
 */
uint16_t COMM_GetDataCount() {
  if (kUsartFlag) {
    //UART
    return USART_GetDataCount();
  } else {
    //USB
    return USB_GetDataCount();
  }
}

/**
 * @brief  Returns only one character of data received by USB or USART.
 */
char COMM_GetChar() {
  if (kUsartFlag) {
    //UART
    return USART_GetChar();
  } else {
    //USB
    return USB_GetChar();
  }
}

/**
 * @brief  Polling transmission is performed by USB or USART.
 */
bool COMM_PollingSend(char *buf) {
  kTimeoutCnt = 0;
  while (!COMM_IsTransmitFree()) {
    if (PollingTimeoutReporter()) {
      SetSystemError(kPollingTimeoutError);
      break;
    }
  }

  uint32_t length = strlen(buf);
  if (kUsartFlag) {
    USART_AnyAddressTransmit(buf, length);
  } else {
    CDC_Transmit_FS((uint8_t*) buf, length);
  }

  kTimeoutCnt = 0;
  while (!COMM_IsTransmitFree()) {
    if (PollingTimeoutReporter()) {
      SetSystemError(kPollingTimeoutError);
      break;
    }
  }

  return true;
}

/**
 * @brief  Monitor timeouts when sending polls.
 */
bool PollingTimeoutReporter() {
  if (kTimeoutCnt > TIMEOUT) {
    kTimeoutCnt = 0;
    return true;
  } else {
    DelayMicrosecond(1);
    kTimeoutCnt++;
    return false;
  }
}
