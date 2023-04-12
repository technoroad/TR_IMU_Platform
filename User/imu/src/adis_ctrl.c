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
static bool kBurstReadFlag = false;

/* Private function prototypes -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
extern BoardParameterList gBoard;

/**
 * @brief  Configure IMU related settings.
 */
bool IMU_Initialization(void) {
  // Hardware reset
  GREEN_ON;
  IMU_RESET_ON;
  DelayMillisecond(500);
  IwdgRefresh();
  GREEN_OFF;
  IMU_RESET_OFF;
  DelayMillisecond(500);
  IwdgRefresh();
  GREEN_ON;

  SPI_SetBaudRate(SPI_SINGLE_PRES);

  // Check the product id of the IMU that the firmware supports
  if (!IMU_VerificationProductId()) {
    return false;
  }

  // Set Decimation rate
  uint32_t freq = (uint32_t) (1.0 / CTRL_PERIOD);
  IMU_WriteData(PAGE3, DEC_RATE, (CONVERSION_RATE / freq) - 1);
  DelayMillisecond(500);

  // Sets the bias collection time
  if (gBoard.auto_average_enable) {
    IMU_SetAverageTime(gBoard.auto_average_time);
  }

// Perform burst read only if 32bit read capability is present.
#if defined(AVAILABLE_32BIT_BURST) && defined(SUPPORT_BURST_FIRM_REV)
#if defined(ADIS1647X)
  uint16_t firm_rev = IMU_ReadData(PAGE3, FIRM_REV);
  if (firm_rev >= SUPPORT_BURST_FIRM_REV) {
    // select 32bit burst read
    uint16_t reg = IMU_ReadData(PAGE0,MSC_CTRL);
    reg |= 1 << 9;
    IMU_WriteData(PAGE0,MSC_CTRL, reg);

    DelayMillisecond(500);
    IwdgRefresh();
    kBurstReadFlag = true;
  }
#elif defined(ADIS1649X)
  IwdgRefresh();  // MSC_CTRL does not exist in the ADIS1649X series.
  Crc32Initialize();  //CRC32 initialize
  kBurstReadFlag = true;
#endif
#endif

#if defined(ADIS1647X)
  // Hard filter setting in ADIS1647X
  IMU_SetHardwareFilter(1, 0, 0, 0, 0, 0);
#elif defined(ADIS1649X)
  // Hard filter setting in ADIS1649X
  IMU_SetHardwareFilter(FILT_BANK_C, FILT_BANK_C,  //
                        FILT_BANK_C, FILT_BANK_C, FILT_BANK_C, FILT_BANK_C);
#endif

  return true;
}

/**
 * @brief  Get Product ID
 */
uint16_t IMU_GetProductId() {
  return IMU_ReadData(PAGE0, PROD_ID);
}

/**
 * @brief  Returns whether burst read is enabled
 */
bool IMU_IsBurstReadEnable() {
  return kBurstReadFlag;
}
/**
 * @brief  Check if specified sensor is connected
 */
bool IMU_VerificationProductId(void) {
  if (IMU_GetProductId() == PRODUCT_ID) {
    return true;
  } else {
    return false;
  }
}

/**
 * @brief  A function to switch between 32bit burst read and single read.
 */
void IMU_Update6AxisData() {
  if (kBurstReadFlag) {
    IMU_Update6AxisDataWithBurstRead();
  } else {
    IMU_Update6AxisDataWithSingleRead();
  }
}

/**
 * @brief  Update sensor bias value
 */
void IMU_BiasCorrectionUpdate(void) {
  IMU_WriteData(PAGE3, GLOB_CMD, 0x0001);
}

/**
 * @brief  Hard filter selection.
 * @note  See the data sheet for details.
 */
void IMU_SetHardwareFilter(int f0, int f1, int f2, int f3, int f4, int f5) {
#if defined(ADIS1647X)
  // Set digital filter
  IMU_WriteData(PAGE3, FILT_CTRL, f0);
#elif defined(ADIS1649X)
  uint16_t filter0 = 0;
  uint16_t filter1 = 0;
  int bank0_arr[5] = { f0, f1, f2, f3, f4 };

  //FILTR_BNK_0
  for (int i = 0; i < 5; i++) {
    if ((bank0_arr[i] <= FILT_BANK_D) && (bank0_arr[i] >= FILT_BANK_A)) {
      filter0 |= (uint16_t) ((bank0_arr[i] | 0x0004) << (i * 3));
    } else {
      filter0 |= (uint16_t) ((bank0_arr[i] & 0x0003) << (i * 3));
    }
  }

  //FILTR_BNK_1
  if ((f5 <= FILT_BANK_D) && (f5 >= FILT_BANK_A)) {
    filter1 |= (uint16_t) (f5 | 0x0004);
  } else {
    filter1 |= (uint16_t) (f5 & 0x0003);
  }

  IMU_WriteData(PAGE3, FILTR_BNK_0, filter0);
  DelayMillisecond(1);
  IMU_WriteData(PAGE3, FILTR_BNK_1, filter1);
  DelayMillisecond(1);

#endif
}

/**
 * @brief  Sets the BiasCorrection time.
 */
void IMU_SetAverageTime(uint32_t sec) {
  double tb = (double) sec / 64.0;
  double tc = tb * CONVERSION_RATE;
  uint32_t multi = 0;
  while (tc > 1.0) {
    tc /= 2;
    multi++;
  }
  if (multi > MAX_TBC) {
    multi = MAX_TBC;
  }
  uint16_t reg = IMU_ReadData(PAGE3, NULL_CNFG);
  reg = (reg & 0xFFF0) | (multi & 0x000F);
  IMU_WriteData(PAGE3, NULL_CNFG, reg);
}

