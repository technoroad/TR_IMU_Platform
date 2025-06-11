/*
 * checksum.c
 *
 *  Created on: Dec 5, 2022
 *      Author: techno-road
 */

/* Includes ------------------------------------------------------------------*/
#include "libraries.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/**
 * @brief  引数sumから１の補数和を求めます。RFC1071に準拠しています。
 * @param[in] sum : 合計値
 * @return 合計値を8ビット上限とした時の1の補数和
 */
uint16_t OneComplimentSum(uint32_t sum) {
  // 16ビットの1の補数和を求める
  while (sum >> 16) {
      sum = (sum & 0xFFFF) + (sum >> 16);
  }

  return (uint16_t) sum;
}

/**
 * @brief  16ビットの1の補数和のチェック。RFC1071に準拠しています。
 * @param[in]  受信したチェックサムを含めた合算値
 * @return  true : チェックサム一致
 *          false : 不一致
 * @note  受信時の専用関数
 */
bool CompareChecksum(uint32_t sum) {
  return OneComplimentSum(sum) == 0xFFFF;
}

/**
 * @brief  チェックサムの作成。RFC1071に準拠しています。
 * @param[in]  データ部分の合算値
 * @return  送信時の専用関数
 */
uint16_t MakeChecksum(uint32_t sum) {
  // 16ビットの1の補数和の1の補数をチェックサムとする
  return OneComplimentSum(sum) ^ 0xFFFF;
}

