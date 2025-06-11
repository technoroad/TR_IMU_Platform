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
#if defined(STM32F765xx)

/* Includes ------------------------------------------------------------------*/
#include "libraries.h"
#include "stm32f7xx_ll_iwdg.h"

/* Private typedef -----------------------------------------------------------*/
// プリスケーラ
#define IWDG_PRC LL_IWDG_PRESCALER_16
// IWDG発動カウンタの初期値
#define IWDG_RELOADVALUE 4095

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/**
 * @brief  IWDGの設定と起動
 * @note  一度起動させると電源が落ちるまで止められません。
 *        "main.c"でMX_IWDG_Initを実行すると起動してしまうので注意
 */
void IWDG_Enable() {
  //ウォッチドッグタイマによってリセットされたか確認する
  if(IWDG_CheckResetFlag()){
    // された
    SetSystemError(kIwdgResetDone);
    CLEAR_BIT(RCC->CSR, RCC_CSR_IWDGRSTF);
  }else{
    // されてない
  }

  LL_IWDG_Enable(IWDG);
  LL_IWDG_EnableWriteAccess(IWDG);
  LL_IWDG_SetPrescaler(IWDG, IWDG_PRC);
  LL_IWDG_SetReloadCounter(IWDG, IWDG_RELOADVALUE);
  while (LL_IWDG_IsReady(IWDG) != 1) {
  }
  LL_IWDG_ReloadCounter(IWDG);

  // デバッグ時にIWDGの停止を許可
  DBGMCU->APB1FZ = DBGMCU->APB1FZ | DBGMCU_APB1_FZ_DBG_IWDG_STOP;
}

/**
 * @brief  IWDGのリロードカウンタのリフレッシュ
 * @note  常時動作する場所で実行してください
 */
void IWDG_Refresh() {
  LL_IWDG_ReloadCounter(IWDG);
}

/**
 * @brief IWDGのクロック源となるLSIの状態を取得します
 */
bool IWDG_IsEnable() {
  // IWDG起動時に自動で起動されるLSIの状態を返す
  return (LL_RCC_LSI_IsReady() != 1);
}

/**
 * @brief  IWDGによってリセットされたか確認します。
 * @return  true : リセットされた
 *          false : リセットされていない
 */
bool IWDG_CheckResetFlag() {
  // ウォッチドッグによってリセットしたか？
  if (READ_BIT(RCC->CSR, RCC_CSR_IWDGRSTF)) {
    // された
    return true;
  }
  // されてない
  return false;
}

#endif
