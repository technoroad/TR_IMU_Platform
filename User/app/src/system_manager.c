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
static SystemStatus kStatus = kStartup;
static bool kDataReady = false;

/* Private function prototypes -----------------------------------------------*/
void StartupFunc();
void StartupWaitFunc();
void UpdatePoseFunc();
void AdiDriverFunc();
void SystemErrorFunc();
void XportsettingFunc();
void StartDataAcquisition();

/* Global variables ----------------------------------------------------------*/
extern BoardParameterList gBoard;

/**
 * @brief  System state transition function
 * @details  It will transition as follows.
 *  kStartup : Transitions at startup and performs initial settings.
 *    When completed, transition to "kStartupWait".
 *    If the sensor ID is incorrect at this time, it transitions to "kFatalError".
 *  kStartupWait : Stops the transition for the set wait time.
 *    When finished, transition to "kUpdatePose" or "kAdiDriver".
 *  kUpdatePose : Acquire data from sensors and estimate posture.
 *    There is no event to transition to "kFatalError" in the current version.
 *  kAdiDriver : Empty in the current version.
 *    The main processing is done with ReadBinaryCmd().
 *  kFatalError : A function to stop when a fatal error occurs.
 *    In the current version, it only transitions from "kStartup".
 */
void SystemManager() {
  // Command paser select
  if (IsEnableStringCommand()) {
    ReadStringCmd();
  } else if (IsEnableAdiDriver()) {
    ReadBinaryCmd();
  }

  switch (kStatus) {
    case kStartup: {
      StartupFunc();
      break;
    }

    case kStartupWait: {
      StartupWaitFunc();
      break;
    }

    case kUpdatePose: {
      UpdatePoseFunc();
      break;
    }

    case kAdiDriver: {
      AdiDriverFunc();
      break;
    }

    case kFatalError: {
      SystemErrorFunc();
      break;
    }

    case kXportSetting: {
      XportsettingFunc();
      break;
    }

    default: {
      SetSystemError(kUnknownError);
      break;
    }
  }
}

/**
 * @brief  Transitions at startup and performs initial settings.
 * @note  When completed, transition to "kStartupWait".
 *  If the sensor ID is incorrect at this time, it transitions to "kFatalError".
 */
void StartupFunc() {
  // Acquisition of CONF switch information
  CheckConfigrationSwitch();

  // IMU initialization
  if (IMU_Initialization()) {
    // Standby enable for bias correction
    if (IsStartupWaitEnable()) {
      // Stop DR pin
      ExtiStop();
      kStatus = kStartupWait;
    } else {
      // Start data acquisition without waiting
      StartDataAcquisition();
    }
  } else {
    // An error occurs if the device ID is not a compatible model
    SetSystemError(kIdIncorrectError);
  }
}

/**
 * @brief  Stops the transition for the set wait time.
 * @note  When finished, transition to "kUpdatePose" or "kAdiDriver".
 */
void StartupWaitFunc() {
  // Skip wait with user button
  if (READ_USER_SW) {
    DelayMillisecond(500);
    StartupWaitDisable();
    StartDataAcquisition();
  }

  StartupWait();

  if (IsStartupWaitComplete()) {
    // Start data acquisition without waiting
    StartDataAcquisition();
  }
}

/**
 * @brief  Acquire data from sensors and estimate posture.
 * @note  There is no event to transition to "kFatalError" in the current version.
 */
void UpdatePoseFunc() {
  // Reset system if user switch is pressed
  if (READ_USER_SW) {
    SendStop();
    DelayMillisecond(500);
    MKAE_MS2016_FilterReset();
  }

  if (kDataReady) {
    kDataReady = false;

    DEBUG0_PIN_HIGH;

    // Acquisition of sensor value and calculation of attitude angle.
    MKAE_MS2016_UpdateAngle();

    // transmit data
    PushImuData();

    DEBUG0_PIN_LOW;
  }
}

/**
 * @brief  Empty in the current version.
 * @note  The main processing is done with ReadBinaryCmd().
 */
void AdiDriverFunc() {
}

/**
 * @brief  kFatalError : A function to stop when a fatal error occurs.
 * @note  In the current version, it only transitions from "kStartup".
 */
void SystemErrorFunc() {
  if (READ_USER_SW) {
    DelayMillisecond(500);
    HAL_NVIC_SystemReset();
  }
}

void XportsettingFunc() {
#if defined(STM32F765xx)
  XportDirectCommunication();
#endif
}

/**
 * @brief  A function to execute when the DR pin is sensed.
 */
void ExtiInterrupt() {
  kDataReady = true;
}

/**
 * @brief  Start acquiring sensor data.
 */
void StartDataAcquisition() {
  if (IsEnableAdiDriver()) {
    // don't use DR pin
    ExtiStop();

    // adi_driver start
    kStatus = kAdiDriver;
  } else if (IsEnableStringCommand()) {
    // Filter initialization
    MKAE_MS2016_Initialize();
    DelayMillisecond(100);

    // Interrupt initiation by DR pin
    ExtiStart();

    // pose update start
    kStatus = kUpdatePose;
  }
}

/**
 * @brief  Enter fatal error status in system manager.
 */
void SetSystemFatalError() {
  kStatus = kFatalError;
}

/**
 * @brief Returns the state of the system manager.
 */
SystemStatus GetSystemStatus() {
  return kStatus;
}

/**
 * @brief  Reset the system manager to
 *         the initial state and redo the settings.
 */
bool ResetSystemStatus() {
  kStatus = kStartup;
  SendStop();
  return true;
}

/**
 * @brief  Put the system into XPORT configuration mode.
 */
void TransitionXportSettingMode() {
  kStatus = kXportSetting;
}
