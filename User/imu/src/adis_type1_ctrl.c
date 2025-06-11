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

/* Global variables ----------------------------------------------------------*/
extern System_Status gSystem;

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define SPI_BUF_SIZE 32

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static volatile uint16_t kSendBuf[SPI_BUF_SIZE] = { 0 };
static volatile uint16_t kReadBuf[SPI_BUF_SIZE] = { 0 };
static ImuDataList *kIMU = &gSystem.data;
static IMU_TypeSetting *kSet = &gSystem.tset;
static IMU_Function *kFunc = &gSystem.func;

/* Private function prototypes -----------------------------------------------*/
static void IMU_SetDecimationRate();
static void IMU_EnableBurstRead();
static void IMU_BiasCorrectionUpdate();
static void IMU_SetHardwareFilter(uint8_t f0);
static void IMU_SetBiasCorrectionTime(uint32_t sec);
static uint16_t IMU_GetDiagnosticFlag();
static uint16_t IMU_GetProductId();
static uint16_t IMU_GetRangeModel();
static void IMU_WriteData(uint8_t page, uint8_t addr, uint16_t data);
static uint16_t IMU_ReadData(uint8_t page, uint16_t addr);
static void IMU_Update6AxisDataWithSingleRead();
static void IMU_Update6AxisDataWithBurstRead();
static void IMU_UpdateGyroBias();

static void kConvertBiasData(uint16_t *data);
static void kConvert32bitDataWithSingleRead(uint16_t *data);
static void kConvert32bitDataWithBurstRead(uint16_t *data);

/* Global variables ----------------------------------------------------------*/

void IMU_RegisterType1Functions(){
  // 初期化
  kFunc->SetDecimationRate         = NULL;
  kFunc->EnableBurstRead           = NULL;
  kFunc->BiasCorrectionUpdate      = NULL;
  kFunc->SetHardwareFilter         = NULL;
  kFunc->SetBiasCorrectionTime     = NULL;
  kFunc->GetDiagnosticFlag         = NULL;
  kFunc->GetProductId              = NULL;
  kFunc->GetRangeModel             = NULL;
  kFunc->WriteData                 = NULL;
  kFunc->ReadData                  = NULL;
  kFunc->Update6AxisDataWithSingleRead = NULL;
  kFunc->Update6AxisDataWithBurstRead  = NULL;
  kFunc->UpdateGyroBias            = NULL;

  // 関数ポインタを設定
  kFunc->SetDecimationRate         = IMU_SetDecimationRate;
  kFunc->EnableBurstRead           = IMU_EnableBurstRead;
  kFunc->BiasCorrectionUpdate      = IMU_BiasCorrectionUpdate;
  kFunc->SetHardwareFilter         = IMU_SetHardwareFilter;
  kFunc->SetBiasCorrectionTime     = IMU_SetBiasCorrectionTime;
  kFunc->GetDiagnosticFlag         = IMU_GetDiagnosticFlag;
  kFunc->GetProductId              = IMU_GetProductId;
  kFunc->GetRangeModel             = IMU_GetRangeModel;
  kFunc->WriteData                 = IMU_WriteData;
  kFunc->ReadData                  = IMU_ReadData;
  kFunc->Update6AxisDataWithSingleRead = IMU_Update6AxisDataWithSingleRead;
  kFunc->Update6AxisDataWithBurstRead  = IMU_Update6AxisDataWithBurstRead;
  kFunc->UpdateGyroBias            = IMU_UpdateGyroBias;
}

static void IMU_EnableBurstRead(){
  uint16_t reg = IMU_ReadData(0,TYPE1_MSC_CTRL);
  reg |= 1 << 9;
  IMU_WriteData(0,TYPE1_MSC_CTRL, reg);

  DelayMillisecond(500);
  kSet->burst_enable = true;
}

static void IMU_SetDecimationRate(){
  IMU_WriteData(0,TYPE1_DEC_RATE, kSet->dec_rate);
}

/**
 * @brief  Update sensor bias value
 */
static void IMU_BiasCorrectionUpdate() {
  IMU_WriteData(0,TYPE1_GLOB_CMD, 0x0001);
}

/**
 * @brief  Hard filter selection.
 * @note  See the data sheet for details.
 */
static void IMU_SetHardwareFilter(uint8_t f0) {
  // Set digital filter
  IMU_WriteData(0,TYPE1_FILT_CTRL, kSet->filter);
  kSet->filter = f0;
}

static uint16_t IMU_GetDiagnosticFlag(){
  return IMU_ReadData(0,TYPE1_DIAG_STAT);
}

static uint16_t IMU_GetProductId(){
  return IMU_ReadData(0,TYPE1_PROD_CMD);
}

static uint16_t IMU_GetRangeModel(){
  return IMU_ReadData(0,TYPE1_RANG_MDL);
}

/**
 * @brief  Sets the BiasCorrection time.
 */
static void IMU_SetBiasCorrectionTime(uint32_t sec) {
  double tb = (double) sec / 64.0;
  double tc = tb * kSet->conversion_rate;
  uint32_t multi = 0;
  while (tc >= 2.0) {
    tc /= 2.0;
    multi++;
  }
  if (multi > kSet->max_tbc) {
    multi = kSet->max_tbc;
  }
  uint16_t reg = IMU_ReadData(0,TYPE1_NULL_CNFG);
  reg = (reg & 0xC7F0) | (multi & 0x000F);
  IMU_WriteData(0,TYPE1_NULL_CNFG, reg);
}


/**
 * @brief  Write to any address
 * @note   The page works only for the ADIS1649X series.
 */
static void IMU_WriteData(uint8_t page,uint8_t addr, uint16_t data) {
  int len = 0;
  kSendBuf[len++] = ((addr | 0x80) << 8) | (data & 0xFF);
  kSendBuf[len++] = ((addr | 0x81) << 8) | ((data >> 8) & 0xFF);

  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) kReadBuf, len);
}

/**
 * @brief  Read any address
 * @note   The page works only for the ADIS1649X series.
 */
static uint16_t IMU_ReadData(uint8_t page,uint16_t addr) {
  // Burst command can not be read
  if (IMU_IsBurstReadCommand(addr)) {
    return 0;
  }

  int len = 0;
  kSendBuf[len++] = (addr << 8) & 0xFF00;
  kSendBuf[len++] = 0x00;

  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) kReadBuf, len);
  return kReadBuf[1];
}

/**
 * @brief  Updating gyro bias.
 */
static void IMU_UpdateGyroBias() {
  int len = 0;
  for (int i = TYPE1_XG_BIAS_LOW; i <= TYPE1_ZG_BIAS_HIGH; i += 2) {
    kSendBuf[len++] = (i << 8) & 0xFF00;
  }
  kSendBuf[len++] = (TYPE1_DATA_CNT << 8) & 0xFF00;
  kSendBuf[len++] = 0x0000;

  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) kReadBuf, len);
  kConvertBiasData((uint16_t*) &kReadBuf[1]);
}

/**
 * @brief  Updating 6-axis data with a burst read
 */
static void IMU_Update6AxisDataWithBurstRead() {
  // Set all data in the transmit buffer to 0.
  for (int i = 0; i < SPI_BUF_SIZE; i++) {
    kSendBuf[i] = 0;
  }

  kSendBuf[0] = kSet->burst_cmd << 8;
  SPI_SetPrescaler(SPI_1MHZ_PRES);
  SPI_WriteReceiveBurstRead((uint16_t*) kSendBuf, (uint16_t*) kReadBuf,
                            kSet->burst_cmd_size);
  SPI_SetPrescaler(SPI_2MHZ_PRES);

  kConvert32bitDataWithBurstRead((uint16_t*) &kReadBuf[1]);
}

/**
 * @brief  Updating 6-axis data with a single read.
 */
static void IMU_Update6AxisDataWithSingleRead() {
  int len = 0;
  for (int i = TYPE1_DIAG_STAT; i <= TYPE1_TEMP_OUT; i += 2) {
    kSendBuf[len++] = (i << 8) & 0xFF00;
  }
  kSendBuf[len++] = (TYPE1_DATA_CNT << 8) & 0xFF00;
  kSendBuf[len++] = 0x0000;

  SPI_WriteReceive((uint16_t*) kSendBuf, (uint16_t*) kReadBuf, len);
  kConvert32bitDataWithSingleRead((uint16_t*) &kReadBuf[1]);
}

/**
 * @brief  Store the data read by IMU_UpdateGyroBias() in the IMU data structure.
 */
static void kConvertBiasData(uint16_t *data) {
  for (int i = 0; i < 3; i++) {
    kIMU->gyro_bias.raw[i] = ((data[i * 2 + 1] << 16) | data[i * 2]);

    kIMU->gyro_bias.fpp[i] =  //
        (int32_t) kIMU->gyro_bias.raw[i] / kSet->gyro_sensi * M_PI / 180.0;
  }
}

/**
 * @brief  Store the data read by
 *         IMU_Update6AxisDataWithSingleRead() in the IMU data structure.
 */
static void kConvert32bitDataWithSingleRead(uint16_t *data) {
  kIMU->diag_stat = data[0];
  for (int i = 0; i < 3; i++) {
    kIMU->gyro.raw[i] = ((data[i * 2 + 2] << 16) | data[i * 2 + 1]);
    kIMU->accl.raw[i] = ((data[i * 2 + 8] << 16) | data[i * 2 + 7]);

    kIMU->gyro.fpp[i] =  //
        (int32_t) kIMU->gyro.raw[i] / kSet->gyro_sensi * M_PI / 180.0;

    kIMU->accl.fpp[i] = (int32_t) kIMU->accl.raw[i] / kSet->accl_sensi;
  }
  kIMU->temperature = (double) ((int16_t) data[13]) * 0.1;
  kIMU->data_cnt = data[14];
}

/**
 * @brief  Store the data read by
 *         IMU_Update6AxisDataWithSingleRead() in the IMU data structure.
 */
static void kConvert32bitDataWithBurstRead(uint16_t *data) {
  kIMU->diag_stat = data[0];
  for (int i = 0; i < 3; i++) {
    kIMU->gyro.raw[i] = ((data[i * 2 + 2] << 16) | data[i * 2 + 1]);
    kIMU->accl.raw[i] = ((data[i * 2 + 8] << 16) | data[i * 2 + 7]);

    kIMU->gyro.fpp[i] =  //
        (int32_t) kIMU->gyro.raw[i] / kSet->gyro_sensi * M_PI / 180.0;

    kIMU->accl.fpp[i] = (int32_t) kIMU->accl.raw[i] / kSet->accl_sensi;
  }
  kIMU->temperature = (double) ((int16_t) data[13]) * 0.1;
  kIMU->data_cnt = data[14];

  uint32_t sum = 0;
  for (int i = 0; i < 15; i++) {
    sum += data[i] & 0xff;
    sum += (data[i] >> 8) & 0xff;
  }

  kIMU->sum = sum;
  kIMU->csum = data[15];
}
