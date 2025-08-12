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
static uint32_t start_tick = 0;
static uint32_t prev_tick = 0;  // 前回の呼び出し時のカウンタ

/* Private function prototypes -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/


// DWTを有効化
void DWT_Init(void) {
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->LAR = 0xC5ACCE55;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void DelayMicrosecond(uint32_t us) {
  start_tick = DWT->CYCCNT;
  while ((DWT->CYCCNT - start_tick) < (us * (SystemCoreClock/1000000)));
}

void DelayMillisecond(uint32_t ms) {
  start_tick = DWT->CYCCNT;
  while ((DWT->CYCCNT - start_tick) < (ms * (SystemCoreClock / 1000)));
}

/* 経過時間をマイクロ秒で返す関数 */
uint32_t ElapsedMicroseconds(void) {
    uint32_t current_tick = DWT->CYCCNT;

    // SystemCoreClock はクロック周波数 [Hz]
    uint32_t us = (current_tick - prev_tick) / (SystemCoreClock / 1000000);
    prev_tick = current_tick;

    return us;
}
