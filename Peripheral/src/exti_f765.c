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

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static UserEventHandlerPtr UserHandler = NULL;
static bool kInterruptAccept = false;

// 使用するタイマーを選択してください
static uint32_t EXTIx = LL_EXTI_LINE_9;

/* Private function prototypes -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/**
 * @brief  EXTIの割り込みハンドラ
 * @note  この関数は"stm32f0xx_it.c"の"EXTI_IRQHandler"関数で呼び出されます。
 */
void ExtiHandler() {
  // 関数ポインタを使用して指定された関数を起動
  if(kInterruptAccept){
    if (UserHandler != NULL) {
      (UserHandler)();
    }
  }else{
    ExtiStop();
  }
}

/**
 * @brief  EXTIのスタート関数
 */
void ExtiStart() {
  kInterruptAccept = true;
  LL_EXTI_EnableIT_0_31(EXTIx);
}

/**
 * @brief  EXTIのストップ関数
 */
void ExtiStop() {
  kInterruptAccept = false;
  LL_EXTI_DisableIT_0_31(EXTIx);
  LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_7);
}

/**
 * @brief  EXTIの割り込み時に呼び出す関数を指定できます。
 * @param  handler : 関数ポインタ
 * @note
 */
void ExtiAttachEventHandler(UserEventHandlerPtr handler) {
  //ユーザハンドラの設定
  UserHandler = handler;
}

#endif
