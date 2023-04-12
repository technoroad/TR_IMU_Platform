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
#ifdef MAHONY
mkAE_IMU mkae_Mah;
static double Kp_Mah = 1.0, Ki_Mah = 0.1;
static double wA_Mah = 1.0;
static double gyro_mah[3], acc_mah[3];
#elif defined(MASUYA)
static mkAE_IMU_MS2016 mkae_ms;
static double wA_ms = 1;
#define FREQ_GT 100.0
#define FREQ_ACC 3.0
#define FERQ_GYRO 30.0
#define FREQ_RELAX 90.0
#endif

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Get6AxisData(double *p_gyro, double *p_acc);

/* Global variables ----------------------------------------------------------*/
extern BoardParameterList gBoard;

/**
 * @brief  Filter initialization
 */
void MKAE_MS2016_Initialize() {
  double gyro[3], acc[3];
  Get6AxisData(gyro, acc);

  // Since the error occurs only for the first time, it is resolved.
#if defined(ADIS1647X)
  IMU_ReadData(PAGE0, DIAG_STAT);
#elif defined(ADIS1649X)
  IMU_ReadData(PAGE0, SYS_E_FLAG);
#endif

#ifdef MAHONY
  mkAE_IMU_Initialize(&mkae_Mah, acc);
#elif defined(MASUYA)
  // Filter initialization
  mkAE_IMU_MS2016_Initialize(&mkae_ms, gyro, acc, 1, 1, 1);
  MKAE_MS2016_SetParam(&mkae_ms);
#endif
}

/**
 * @brief  Calculate angle from acceleration and angular acceleration.
 */
void MKAE_MS2016_UpdateAngle() {
  double gyro[3], acc[3], angle[3];

  // Calculate angular velocity and acceleration values.
  Get6AxisData(gyro, acc);

#ifdef MAHONY
  // Output of attitude matrix
  mkAE_IMU_MahonyECF_AM(&mkae_Mah, gyro, acc, wA_Mah,  //
                        Kp_Mah, Ki_Mah, CTRL_PERIOD);
  // Output of Euler angles from attitude matrix
  mk_Cmpt_ZYXangle_AM(mkae_Mah.m_att, angle);

  // Save value only for MAHONY
  for (int i = 0; i < 3; i++) {
    gyro_mah[i] = gyro[i];
    acc_mah[i] = acc[i];
  }

#elif defined(MASUYA)
  // Output of attitude matrix
  mkAE_IMU_MS2016_CF_AM(&mkae_ms, gyro, acc, wA_ms, gBoard.gain_p,
                        gBoard.gain_i,
                        CTRL_PERIOD);
  // Output of Euler angles from attitude matrix
  mk_Cmpt_ZYXangle_AM(mkae_ms.mkae.m_att, angle);
#endif

  // Output of yaw pitch roll from attitude matrix
  ImuDataList *list = IMU_GetDataList();
  list->yaw = angle[0] * 180.0 / M_PI;
  list->pitch = angle[1] * 180.0 / M_PI;
  list->roll = angle[2] * 180.0 / M_PI;

  SystemError error = DetectSensorError(list->diag_stat);
  if (error != kNoneError) {
    SetSystemError(error);
  }
}

/**
 * @brief  Set the output values of yaw, pitch and roll to 0.
 */
void MKAE_MS2016_FilterReset() {
  double gyro[3], acc[3];

#ifdef MAHONY
  for (int i = 0; i < 3; i++) {
    gyro[i] = gyro_mah[i];
    acc[i] = acc_mah[i];
  }

  mkAE_IMU_Reset(&mkae_Mah, gyro, acc);
#elif defined(MASUYA)
  for (int i = 0; i < 3; i++) {
    gyro[i] = *mkae_ms._gyro_mes[i];
    acc[i] = *mkae_ms._acc_mes[i];
  }

  mkAE_IMU_MS2016_Reset(&mkae_ms, gyro, acc);
#endif
}

/* Sensor dynamics */
void MKAE_MS2016_SetParam(mkAE_IMU_MS2016 *mkae_ms) {
#ifdef MASUYA
  int i;
  /* Filter for the estimate of the acceleration */
  for (i = 0; i < 3; i++) {
    mk_Set_Param_1stOrderFilter_LP(mkae_ms->param_EstAcc0Fil[i],  //
        1.0, FREQ_ACC, CTRL_PERIOD);
  }

  /* Filter for the accelerometer's measurement */
  for (i = 0; i < 3; i++) {
    mk_Set_Param_1stOrderFilter_LP(mkae_ms->param_MesAccFil[i],  //
        1.0, FREQ_RELAX, CTRL_PERIOD);
  }

  /* Filter for the gyroscope's measurement */
  for (i = 0; i < 3; i++) {
    mk_Set_Param_1stOrderFilter_PC(mkae_ms->param_MesgyroFil[i],  //
        1.0, FERQ_GYRO, FREQ_RELAX, CTRL_PERIOD);
  }
#endif
}

/**
 * @brief  Calculate angular velocity and acceleration values.
 */
void Get6AxisData(double *gyro, double *accl) {
  IMU_Update6AxisData();
//  IMU_UpdateGyroBias();

  ImuDataList *list = IMU_GetDataList();

#if defined(ADIS1647X)
  // Checksum judgment when burst read is enabled.
  if (IMU_IsBurstReadEnable()) {
    if (list->csum != list->sum) {
      // However, the current version does not stop working.
      SetSystemError(kBurstReadChecksumError);
    }
  }
#elif defined(ADIS1649X)
  // Checksum judgment when burst read is enabled.
  if (IMU_IsBurstReadEnable()) {
    if (list->recv_crc32 != list->calc_crc32) {
      // However, the current version does not stop working.
      SetSystemError(kBurstReadChecksumError);
    }
  }
#endif

  // Convert to acceleration[m/s2]
  for (int i = 0; i < 3; i++) {
    gyro[i] = list->gyro.fpp[i];
    accl[i] = list->accl.fpp[i] * STANDARD_GRAVITY;
  }

  // Calculate acceleration synthesis[m/s2]
  double n = sqrt(accl[0] * accl[0] + accl[1] * accl[1] + accl[2] * accl[2]);
  if (n > 0.0001) {
    accl[0] /= n;
    accl[1] /= n;
    accl[2] /= n;
  } else {
    accl[0] = 0;
    accl[1] = 0;
    accl[2] = 0;
  }
  list->accl_synthesis = n;

  if (gBoard.auto_average_enable) {
    static double peak = 0;
    static uint32_t cnt = 0;
    static uint32_t freq = (uint32_t) (1.0 / CTRL_PERIOD);

    uint32_t avg_tick = freq * gBoard.auto_average_time;
    cnt++;
    double vib = fabs(list->accl_synthesis - STANDARD_GRAVITY);
    if (peak < vib) {
      peak = vib;
    }

    if (vib >= gBoard.vibration_threshold) {
      peak = 0;
      cnt = 0;
    }

    list->vibration_peak = peak;

    if (cnt > avg_tick) {
      IMU_BiasCorrectionUpdate();
      cnt = 0;
      peak = 0;
    }
  }
}
