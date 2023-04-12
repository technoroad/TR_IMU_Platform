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

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
#if defined(STM32F070xB)

/**
 * @brief  Set the standby time in 1us intervals.
 */
void __attribute__((optimize("O0")))DelayMicrosecond(uint32_t delay) {
  while (delay > 0) {
    if(delay > 1){
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    }else if(delay == 1){
      asm("NOP");
      break;
    }
    delay--;
  }
}

/**
 * @brief  Set the standby time in 1ms intervals.
 */
void __attribute__((optimize("O0")))DelayMillisecond(uint32_t delay) {
  //単位変換
  delay *= 1000;

  while (delay > 0) {
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    delay--;
  }
}
#elif defined(STM32F765xx)
/**
 * @brief  Set the standby time in 1us intervals.
 */
void __attribute__((optimize("O0")))DelayMicrosecond(uint32_t delay) {
  while (delay > 0) {
    if(delay > 1){
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    }else if(delay == 1){
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
      break;
    }
    delay--;
  }
}

/**
 * @brief  Set the standby time in 1ms intervals.
 */
void __attribute__((optimize("O0")))DelayMillisecond(uint32_t delay) {
  //単位変換
  delay *= 1000;

  while (delay > 0) {
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP"); asm("NOP");
    asm("NOP");
    delay--;
  }
}
#endif
