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

#define CLOCK_FREQ 216000000UL
#define TARGET_FREQ 2000.0

void find_best_psc_arr(uint16_t *best_psc, uint16_t *best_arr,
                       double *best_freq, double *best_error) {
  *best_error = 1e9;

  for (uint32_t psc = 1; psc <= 65535; psc++) {
    // ARRを計算（1減算はタイマの仕様）
    uint32_t arr = (uint32_t) (CLOCK_FREQ / ((psc + 1) * TARGET_FREQ)) - 1;

    if (arr == 0 || arr > 65535) {
      continue;  // 範囲外は無視
    }

    double freq = (double) CLOCK_FREQ / ((psc + 1) * (arr + 1));
    double error = fabs(freq - TARGET_FREQ);

    if (error < *best_error) {
      *best_error = error;
      *best_psc = (uint16_t) psc;
      *best_arr = (uint16_t) arr;
      *best_freq = freq;
    }
  }
}

/**
 * @brief  Settings that are only performed at startup.
 */
void setup() {
//  uint16_t best_psc = 0;
//  uint16_t best_arr = 0;
//  double best_freq = 0.0;
//  double best_error = 0.0;
//  find_best_psc_arr(&best_psc, &best_arr, &best_freq, &best_error);

  DWT_Init();
  // Read parameters from FLASH memory.
  LoadSettingsFromFlash(false);
}

/**
 * @brief  Write a function to loop.
 */
void loop() {
  while (1) {
    // watchdog refresh
    IWDG_Refresh();

    // Controller for correct operation
    SystemManager();

    // Set data when not using transmission
    if (COMM_IsTransmitFree()) {
      COMM_TransmitStart();
    }

    __WFI();
  }
}
