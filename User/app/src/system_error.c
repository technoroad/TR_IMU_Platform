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
static SystemError kError = kNoneError;

/* Private function prototypes -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/**
 * @brief  Set system error.
 * @details  Stops IMU data transmission in case of fatal error,
 *           but does not stop in other errors.
 *           Fatal errors are currently only detected at startup time.
 *           (Only fatal error is "sensor id incorrect error")
 */
void SetSystemError(SystemError error) {
  // Change "NoneError" to "UnknownError".
  if (error == kNoneError) {
    kError = kUnknownError;
  }

  // Stop communication in case of fatal error.
  if ((error & FATAL_ERROR_MASK) == FATAL_ERROR_MASK) {
    SetSystemFatalError();
    COMM_ResetDoubleBuffer();
    SendStop();
    ExtiStop();
  }

  // error variable for reporting and "RED LED ON"
  RED_ON;
  kError = error;
}

/**
 * @brief  Returns an error number.
 */
SystemError GetSystemError() {
  return kError;
}

/**
 * @brief  Eliminate the error.
 * @details  In case of fatal error, the setting is done from the beginning.
 */
void ResetSystemError() {
  if ((kError & FATAL_ERROR_MASK) == FATAL_ERROR_MASK) {
    ResetSystemStatus();
  }
  kError = kNoneError;
  RED_OFF;
}

/**
 * @brief  Returns whether the error number in the argument is a fatal error.
 */
bool IsFatalError(SystemError error) {
  if ((error & FATAL_ERROR_MASK) == FATAL_ERROR_MASK) {
    return true;
  } else {
    return false;
  }
}

/*
 * @brief  Detects IMU sensor errors.
 */
SystemError DetectSensorError(uint16_t flag) {
  SystemError error = kNoneError;

#ifdef ADIS1647X
  if (flag & ADIS1647X_CLOCKERROR) {
    error = k1647xClockError;
  } else if (flag & ADIS1647X_MEMORYERROR) {
    error = k1647xMemoryError;
  } else if (flag & ADIS1647X_SENSORFAILURE) {
    error = k1647xSensorFailure;
  } else if (flag & ADIS1647X_STANDBYMODE) {
    error = k1647xStandbyMode;
  } else if (flag & ADIS1647X_SPICOMMERROR) {
    error = k1647xSpiCommError;
  } else if (flag & ADIS1647X_MEMORYUPDATEFAILURE) {
    error = k1647xMemoryUpdateFailure;
  } else if (flag & ADIS1647X_DATAPATHOVERRUN) {
    error = k1647xDataPathOverrun;
  }
#elif defined(ADIS1649X)
  if (flag & ADIS1649X_WATCHDOGTIMER) {
    error = k1649xWatchdogTimer;
  } else if (flag & ADIS1649X_SYNCERROR) {
    error = k1649xSyncError;
  } else if (flag & ADIS1649X_PROCESSINGOVERRUN) {
    error = k1649xProcessingOverrun;
  } else if (flag & ADIS1649X_MEMORYUPDATEFAILURE) {
    error = k1649xMemoryUpdateFailure;
  } else if (flag & ADIS1649X_SENSORFAILURE) {
    error = k1649xSensorFailure;
  } else if (flag & ADIS1649X_SPICOMMERROR) {
    error = k1649xSpiCommError;
  } else if (flag & ADIS1649X_SRAMERROR) {
    error = k1649xSramError;
  } else if (flag & ADIS1649X_BOOTMEMORYFAILURE) {
    error = k1649xBootMemoryFailure;
  }
#endif
  return error;
}
