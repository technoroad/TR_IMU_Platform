/*
 * imu_select.c
 *
 *  Created on: 2025/06/11
 *      Author: techno-road
 */

/* Includes ------------------------------------------------------------------*/
#include "libraries.h"

/* Global variables ----------------------------------------------------------*/
System_Status gSystem = { 0 };

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PROD_ID_ERROR ((id < 16470) || (id > 20000))

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static IMU_TypeSetting *kSet = &gSystem.tset;
static IMU_Function *kFunc = &gSystem.func;

/* Private function prototypes -----------------------------------------------*/
static bool kPorductSelector();

/**
 * @brief  Configure IMU related settings.
 */
bool IMU_Initialization(void) {
  // Hardware reset
  GREEN_ON;
  IMU_RESET_ON;
  DelayMillisecond(500);
  GREEN_OFF;
  IMU_RESET_OFF;
  DelayMillisecond(500);
  GREEN_ON;

  if(!kPorductSelector()){
    return false;
  }

  kFunc->SetDecimationRate();
  DelayMillisecond(500);

  // Sets the bias collection time
  kFunc->SetBiasCorrectionTime(gSystem.conf.auto_update_time);

  // Hard filter setting
  kFunc->SetHardwareFilter(kSet->filter);

  return true;
}

/**
 * @brief  A function to switch between 32bit burst read and single read.
 */
void IMU_Update6AxisData() {
  if (kSet->burst_enable) {
    kFunc->Update6AxisDataWithBurstRead();
  } else {
    kFunc->Update6AxisDataWithSingleRead();
  }
}

/**
 * @brief  Get Product ID
 */
uint16_t IMU_GetProductId() {
  return kSet->product_id;
}

/*
 * @brief  Get sensor sensitivity
 */
void IMU_GetSensitivity(double *gyro_sensi, double *accl_sensi) {
  *gyro_sensi = kSet->gyro_sensi;
  *accl_sensi = kSet->accl_sensi;
}

/**
 * @brief  Returns whether it is a burst read from the page and address.
 * @note   The page works only for the ADIS1649X series.
 */
bool IMU_IsBurstReadCommand(uint16_t addr) {
  if (addr == kSet->burst_cmd) {
    return true;
  }
  return false;
}

bool kPorductSelector() {
  SPI_SetPrescaler(SPI_1MHZ_PRES);
  IMU_RegisterType1Functions();
  uint16_t id = kFunc->GetProductId();

  kSet->type = IMU_Type1;
  kSet->tstall = 16;
  kSet->conversion_rate = 2000;
  kSet->dec_rate = kSet->conversion_rate / CTRL_FREQ - 1 ;
  kSet->dt = 1.0 / (kSet->conversion_rate / (kSet->dec_rate + 1));
  kSet->max_tbc = 12;
  kSet->filter = 1;
  kSet->burst_enable = false;
  kSet->burst_cmd = TYPE1_BURST_CMD;
  kSet->burst_cmd_size = 17;

  if (PROD_ID_ERROR) {
    IMU_RegisterType2Functions();
    id = kFunc->GetProductId();
    if (PROD_ID_ERROR) {
      return false;
    }
  }

  uint16_t model = kFunc->GetRangeModel();

  if (id == 16470) {
    // case by ADIS16470
    kSet->gyro_sensi = 655360.0f;
    kSet->accl_sensi = 52428800.0f;
    kSet->gyro_noise_density = 0.008f;
  } else if (id == 16475) {
    // case by ADIS16475
    if(model == MDL_1BMLZ){
      kSet->gyro_sensi = 10485760.0f;
      kSet->gyro_noise_density = 0.003f;
    }else if(model == MDL_2BMLZ){
      kSet->gyro_sensi = 2621440.0f;
      kSet->gyro_noise_density = 0.003f;
    }else if(model == MDL_3BMLZ){
      kSet->gyro_sensi = 665360.0f;
      kSet->gyro_noise_density = 0.007f;
    }

    kSet->accl_sensi = 262144000.0f;
  } else if (id == 16477) {
    // case by ADIS16477-2
    if(model == MDL_1BMLZ){
      kSet->gyro_sensi = 10485760.0f;
      kSet->gyro_noise_density = 0.003f;
    }else if(model == MDL_2BMLZ){
      kSet->gyro_sensi = 2621440.0f;
      kSet->gyro_noise_density = 0.003f;
    }else if(model == MDL_3BMLZ){
      kSet->gyro_sensi = 665360.0f;
      kSet->gyro_noise_density = 0.007f;
    }

    kSet->accl_sensi = 52428800.0f;
    if (kFunc->ReadData(0,TYPE1_FIRM_REV) >= 0x134) {
      // enable 32bit burst read
      kFunc->EnableBurstRead();
    }
  } else if (id == 16500) {
    // case by ADIS16500
    kSet->gyro_sensi = 655360.0f;
    kSet->accl_sensi = 5351254.0f;
    kSet->gyro_noise_density = 0.007f;
    kSet->burst_cmd_size = 18;

    // enable 32bit burst read
    kFunc->EnableBurstRead();

  } else if (id == 16505) {
    // case by ADIS16505
    if(model == MDL_1BMLZ){
      kSet->gyro_sensi = 10485760.0f;
      kSet->gyro_noise_density = 0.003f;
    }else if(model == MDL_2BMLZ){
      kSet->gyro_sensi = 2621440.0f;
      kSet->gyro_noise_density = 0.003f;
    }else if(model == MDL_3BMLZ){
      kSet->gyro_sensi = 665360.0f;
      kSet->gyro_noise_density = 0.007f;
    }

    kSet->accl_sensi = 26756268.0f;
    kSet->burst_cmd_size = 18;

    // enable 32bit burst read
    kFunc->EnableBurstRead();

  } else if (id == 16575) {
    // case by ADIS16575
    if(model == MDL_1BMLZ){
      /* none model */
    }else if(model == MDL_2BMLZ){
      kSet->gyro_sensi = 2621440.0f;
      kSet->gyro_noise_density = 0.003f;
    }else if(model == MDL_3BMLZ){
      kSet->gyro_sensi = 665360.0f;
      kSet->gyro_noise_density = 0.007f;
    }
    kSet->accl_sensi = 262144000.0f;
    kSet->burst_cmd_size = 18;
    kSet->tstall = 5;

    // enable 32bit burst read
    kFunc->EnableBurstRead();
  }else if (id == 16495) {
    kSet->type = IMU_Type2;
    kSet->tstall = 5;
    kSet->conversion_rate = 4250;
    kSet->dec_rate = kSet->conversion_rate / CTRL_FREQ - 1 ;
    kSet->dt = 1.0 / (kSet->conversion_rate / (kSet->dec_rate + 1));
    kSet->max_tbc = 13;
    kSet->filter = FILT_BANK_C;
    kSet->burst_cmd = TYPE2_BURST_CMD;
    kSet->burst_cmd_size = TYPE2_BURST_CMD_SIZE;

    if(model == MDL_1BMLZ){
      kSet->gyro_sensi = 10485760.0f;
      kSet->gyro_noise_density = 0.002f;
    }else if(model == MDL_2BMLZ){
      kSet->gyro_sensi = 2621440.0f;
      kSet->gyro_noise_density = 0.0022f;
    }else if(model == MDL_3BMLZ){
      kSet->gyro_sensi = 665360.0f;
      kSet->gyro_noise_density = 0.0042f;
    }
    kSet->accl_sensi = 262144000.0f;

    kFunc->EnableBurstRead();
  }

  kSet->product_id = id;

  //TSTALLの設定
  SPI_SetTStall(kSet->tstall);

  if(kSet->type == IMU_Type1){
    SPI_SetPrescaler(SPI_2MHZ_PRES);
  }else if(kSet->type == IMU_Type2){
    SPI_SetPrescaler(SPI_3500KHZ_PRES);
  }

  // センサの設定が読み込めたかチェック
  if (kSet->gyro_sensi != 0.0) {
    return true;
  } else {
    return false;
  }
}

