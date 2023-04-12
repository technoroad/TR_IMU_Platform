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
#define SEC_TICK (uwTick / 1000)
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static bool kStartupWaitEnableFlag = false;
static bool kStartupWaitCompleteFlag = false;
static uint32_t kPastTick = 0;
static uint32_t kEndTick = 0;

/* Private function prototypes -----------------------------------------------*/
void StartupWaitTerminate();

/* Global variables ----------------------------------------------------------*/
extern BoardParameterList gBoard;
extern __IO uint32_t uwTick;

/**
 * @brief  A function that makes "kStartupWaitCompleteFlag"
 *         true when the specified time comes only at startup.
 * @note  There is no wait loop inside this function.
 */
void StartupWait() {
  if (kStartupWaitEnableFlag) {
    // Updated every 1 second
    if (kPastTick != SEC_TICK) {
      IMU_BiasCorrectionUpdate();
      IMU_UpdateGyroBias();

      ImuDataList *list = IMU_GetDataList();

      double sum = 0;
      sum += list->gyro_bias.fpp[0];
      sum += list->gyro_bias.fpp[1];
      sum += list->gyro_bias.fpp[2];

      // Wait until the IMU bias register is updated.
      if (sum > 0.0) {
        // Wait until the remaining time reaches zero.
        if (!GetStartupWaitTimeLeft()) {
          StartupWaitTerminate();
        }
      }
    }
    kPastTick = SEC_TICK;
  } else {
    // Abort the wait.
    StartupWaitTerminate();
  }
}

/**
 * @brief  Returns the remaining wait time in seconds.
 */
int GetStartupWaitTimeLeft() {
  int now = kEndTick - SEC_TICK;
  if (now < 0) {
    now = 0;
  }
  return now;
}

/**
 * @brief  Enable wait.
 */
void StartupWaitEnable() {
  if (!IsStartupWaitComplete()) {
    kStartupWaitEnableFlag = true;
    kStartupWaitCompleteFlag = false;
    uwTick = 0;
    kPastTick = SEC_TICK;
    kEndTick = SEC_TICK + gBoard.startup_time;
    BLUE_ON;
  }
}

/**
 * @brief  Disable wait.
 */
void StartupWaitDisable() {
  kStartupWaitEnableFlag = false;
  kStartupWaitCompleteFlag = false;
  kPastTick = 0;
  kEndTick = 0;
  BLUE_OFF;
}

/**
 * @brief  Returns the enable flag for waits on startup.
 */
bool IsStartupWaitEnable() {
  return kStartupWaitEnableFlag;
}

/**
 * @brief  Returns the wait completion flag at startup.
 */
bool IsStartupWaitComplete() {
  return kStartupWaitCompleteFlag;
}

/**
 * @brief  A function that completes waits at startup.
 */
void StartupWaitTerminate() {
  kStartupWaitEnableFlag = false;
  kStartupWaitCompleteFlag = true;
  BLUE_OFF;
}
