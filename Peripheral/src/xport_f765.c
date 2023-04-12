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
#define UART_BUFSIZE 1024
#define USB_BUFSIZE 64

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/**
 * @brief  Make xport and USB hosts communicate through STM32.
 * @note  Actually, USB and UART communication are mutually converted.
 */
void XportDirectCommunication(){
  if(USART_IsTransmitFree()){
    uint32_t len =USB_GetDataCount();
    uint8_t buf[UART_BUFSIZE] ={0};
    if(len){
      if(len > UART_BUFSIZE)len =UART_BUFSIZE;

      for(int i =0;i<len;i++){
        buf[i] = USB_GetChar();
      }

      USART_BusyTimeout();
      USART_TransmitBufferReset();
      USART_PushWriteBuffer(buf, len);
      USART_TransmitStart();
    }
  }

  if(USB_IsTransmitFree()){
    uint32_t len = USART_GetDataCount();
    uint8_t buf[USB_BUFSIZE] ={0};
    if(len){
      if(len > USB_BUFSIZE)len =USB_BUFSIZE;

      for(int i =0;i<len;i++){
        buf[i] = USART_GetChar();
      }

      USB_TransmitBufferReset();
      USB_PushWriteBuffer(buf, len);
      USB_TransmitStart();
    }
  }
}

/**
 * @brief  Make settings for direct XPORT settings.
 */
void XportDirectCommunicationSetting(){
  DisableStringCommand();
  DisableAdiDriver();
  StartupWaitDisable();
  USART_TransmitDmaEnable();
  USART_ReceiveDmaEnable();
  USART_ChangeBaudRate(9600);
  TransitionXportSettingMode();
}

#endif
