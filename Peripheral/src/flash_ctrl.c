/*
 * flash_ctrl.c
 *
 *  Created on: May 7, 2024
 *      Author: techno-road
 */

/* Includes ------------------------------------------------------------------*/
#include "libraries.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define BACKUP_ADDR (uint32_t)0x08008000
#define CSUM_SIZE 2
#define ALIGN 2

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static Flash_Settings kFlash = { 0 };
static uint8_t *kPtr = (uint8_t*) &kFlash;

/* Private function prototypes -----------------------------------------------*/
uint32_t kMakeSum(uint8_t *data, uint32_t size);

/* Global variables ----------------------------------------------------------*/
extern System_Status gSystem;

/**
 * @brief フラッシュメモリから設定を読み込む関数
 *
 * この関数は、マイコンの起動時にフラッシュメモリから設定を読み込みます。
 *
 * @param init_flg true 強制的に設定値を初期化します
 * @return 読み込んだ設定が正常に完了した場合はtrue、それ以外の場合はfalseを返します。
 */
void LoadSettingsFromFlash(bool init_flg) {
  uint32_t size = sizeof(kFlash);

  // フラッシュメモリからkFlashを読み込む
  FLASH_Load(BACKUP_ADDR, kPtr, size);

  // csum生成
  uint32_t sum = kMakeSum(kPtr, size - CSUM_SIZE);
  sum += kFlash.csum;  // csumだけ分解しないで足す

  // csumが相違するか、バージョンが一致しなければkFlashを初期化したのちフラッシュを上書きする
  if (!CompareChecksum(sum) || kFlash.version != VERSION || init_flg) {
    SetInitializeSettingsToStruct();
    SaveSettingsToFlash();
  }

  // 上位PCへ送信用の構造体にも反映
  memcpy(&gSystem.conf, &kFlash.conf, sizeof(kFlash.conf));
}

/**
 * @brief  kFlashをフラッシュメモリに書き込みます
 */
bool SaveSettingsToFlash() {
  //csumを生成
  uint32_t sum = kMakeSum(kPtr, sizeof(kFlash) - CSUM_SIZE);
  kFlash.csum = MakeChecksum(sum);

  // Flashに書き込む
  return FLASH_Store(BACKUP_ADDR, kPtr, sizeof(kFlash), ALIGN);
}

// 初期化関数
void SetInitializeSettingsToStruct() {
  memset(&kFlash, 0, sizeof(kFlash));  // 全体をゼロ初期化

  kFlash.conf.gain_p = GAIN_P_INIT;
  kFlash.conf.gain_i = GAIN_I_INIT;
  kFlash.conf.transmit_prescaler = TRANSMIT_PRESCALER_INIT;
  kFlash.conf.startup_time = STARTUP_TIME_INIT;
  kFlash.conf.auto_update_enable = AUTO_UPDATE_ENABLE_INIT;
  kFlash.conf.auto_update_time = AUTO_UPDATE_TIME_INIT;
  kFlash.conf.stationary_observe_enable = ST_OBSERVE_ENABLE_INIT;
  kFlash.conf.stational_accl_thresh = ST_ACCL_INIT;
  kFlash.conf.stational_gyro_thresh = ST_GYRO_INIT;
  kFlash.version = VERSION;

  // csum生成
  uint32_t sum = kMakeSum(kPtr, sizeof(kFlash) - CSUM_SIZE);
  kFlash.csum = MakeChecksum(sum);
}

/**
 * @brief  フラッシュに保存する設定を更新します。
 */
void SetFlashImuSetting(IMU_Config *config) {
  memcpy(&kFlash.conf, config, sizeof(*config));
}

/**
 * @brief  フラッシュに保存する構造体のポインタを返します。
 */
Flash_Settings* GetFlashSettingPtr() {
  return &kFlash;
}

/**
 * @brief  合計値の算出
 */
uint32_t kMakeSum(uint8_t *data, uint32_t size) {
  uint32_t sum = 0;
  for (int i = 0; i < size; i++) {
    sum += data[i];
  }
  return sum;
}
