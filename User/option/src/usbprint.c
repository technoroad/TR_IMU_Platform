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
#define USB_TIME_OUT 1000

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
// 送信バッファ
static uint8_t kSendBuf[SEND_BUFFER_SIZE] = { 0 };
// 送信バッファのライトインデックス
static int32_t kWriteIndex = 0;

/* Private function prototypes -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
extern USBD_HandleTypeDef hUsbDeviceFS;

bool USB_IsTransmitFree() {
  if (((USBD_CDC_HandleTypeDef*) (hUsbDeviceFS.pClassData))->TxState == 0) {
    return true;
  } else {
    return false;
  }
}

bool USB_PushWriteBuffer(uint8_t *buf, uint32_t length) {
  // データ数チェック
  if (length > SEND_BUFFER_SIZE) {
    return false;
  }

  // 送信バッファへ入力
  for (int i = 0; i < length; i++) {
    kSendBuf[i] = buf[i];
  }
  kWriteIndex += length;

  return true;
}

void USB_TransmitStart() {
  CDC_Transmit_FS(kSendBuf, kWriteIndex);
}

void USB_TransmitBufferReset() {
  if (kWriteIndex > 0) {
    memset(kSendBuf, 0, sizeof(kSendBuf));
    kWriteIndex = 0;
  }
}

//==============================================================================
// String output with USB
//==============================================================================
bool USB_puts(char *buf) {
  // 送信がビジーでなければ送信開始
  if (USB_IsTransmitFree()) {
    uint32_t length = strlen(buf);
    USB_TransmitBufferReset();
    USB_PushWriteBuffer((uint8_t*) buf, length);
    USB_TransmitStart();
    return true;
  } else {
    return false;
  }
}

//==============================================================================
// Character string output specified with USB (string, length of characters)
//==============================================================================
bool USB_putd(char *buf, uint8_t length) {
  // 送信がビジーでなければ送信開始
  if (USB_IsTransmitFree()) {
    USB_TransmitBufferReset();
    USB_PushWriteBuffer((uint8_t*) buf, length);
    USB_TransmitStart();
    return true;
  } else {
    return false;
  }
}

//==============================================================================
// Convert integer value to string and transmission.
//==============================================================================
void USB_putn(int32_t num, uint8_t len) {
  char *s = str_putn(num, len);
  USB_puts(s);
}

//==============================================================================
// Convert integer value to hexadecimal string and transfer.
//==============================================================================
void USB_putx(uint32_t num, uint8_t len) {
  char *s = str_putx(num, len);
  USB_puts(s);
}

//==============================================================================
// Convert float to string and transfer.
//==============================================================================
void USB_putf(float val, int bp, int ap) {
  USB_puts(str_putf(val, bp, ap));
}

//==============================================================================
// Convert double to string and transfer.
//==============================================================================
void USB_putlf(float val, int ap) {
  USB_puts(str_putlf(val, ap));
}

//==============================================================================
// Get 1 character from ring buffer.
//==============================================================================
uint8_t USB_GetChar() {
  return USB_PullChar();
}

