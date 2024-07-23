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
BoardParameterList gBoard;

/**
 * @brief  Load initial values into configuration variables.
 */
void InitializeParameters() {
  uint32_t size = sizeof(gBoard);
  gBoard.version = VERSION;
  gBoard.gain_p = GAIN_P_INIT;
  gBoard.gain_i = GAIN_I_INIT;
  gBoard.transmit_prescaler = TRANSMIT_PRESCALER_INIT;
  gBoard.startup_time = STARTUP_TIME_INIT;
  gBoard.auto_update_enable = AUTO_UPDATE_ENABLE_INIT;
  gBoard.auto_update_time = AUTO_UPDATE_TIME_INIT;
  gBoard.stationary_observe_enable = ST_OBSERVE_ENABLE_INIT;
  gBoard.stational_accl_thresh = ST_ACCL_INIT;
  gBoard.stational_gyro_thresh = ST_GYRO_INIT;
  gBoard.csum = Make16bitChecksum((uint8_t*) &gBoard, size - CSUM_SIZE);
}
