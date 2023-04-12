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

// DMAの送信完了が起きなかった時に強制的に送信終了させるタイムアウト
#define USART_SEND_TIMOUT_TICK 1000
// 受信バッファのサイズ
#define RECV_BUFFER_SIZE 512

//使用するDMA
#define DMAx DMA1
//使用するUSART
#define USARTx UART4
//送信用の関数
#define TX_Stream LL_DMA_STREAM_4
#define LL_DMA_IsActiveFlag_TX LL_DMA_IsActiveFlag_TC4
#define LL_DMA_ClearFlag_TX LL_DMA_ClearFlag_TC4
//受信用の関数
#define RX_Stream LL_DMA_STREAM_2
#define LL_DMA_IsActiveFlag_RX LL_DMA_IsActiveFlag_TC2
#define LL_DMA_ClearFlag_RX LL_DMA_ClearFlag_TC2
#define LL_DMA_NDTR DMA1_Stream2->NDTR
#define GetUSARTxClockFreq LL_RCC_GetUARTClockFreq(LL_RCC_UART4_CLKSOURCE)

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
// 受信リングバッファ
static char kReadBuf[RECV_BUFFER_SIZE] = { 0 };
// 受信バッファのリードインデックス
static int32_t kReadIndex = 0;
// 送信バッファ
static char kSendBuf[SEND_BUFFER_SIZE] = { 0 };
// 送信バッファのライトインデックス
static int32_t kWriteIndex = 0;
// 送信状態のステータス
static bool kUsartBusy = false;
// タイムアウトのカウンタ
static uint32_t kTimeoutCnt = 0;

/* Private function prototypes -----------------------------------------------*/

/***
 * @brief  USARTの割り込み関数
 */
void USART_Handler(void) {
  // 送信完了割り込み
  if (LL_USART_IsActiveFlag_TC(USARTx) == 1) {
    // フラグリセット
    LL_USART_ClearFlag_TC(USARTx);
    // uartの開放
    kUsartBusy = false;
  }
}

/***
 * @brief  USARTの送信完了DMA割り込み関数
 */
void USART_TransmitDmaHandler(void) {
  // 送信完了割り込み
  if (LL_DMA_IsActiveFlag_TX(DMAx) == 1) {
    // 送信完了割り込みのリセット
    LL_DMA_ClearFlag_TX(DMAx);
  }
}

/***
 * @brief  DMA interrupt function for USART receive complete
 */
void USART_ReceiveDmaHandler(void) {
  // Receive complete interrupt
  if (LL_DMA_IsActiveFlag_RX(DMAx) == 1) {
    // Reset receive complete interrupt
    LL_DMA_ClearFlag_RX(DMAx);
  }
}

/***
 * @brief  送信DMAの開始
 */
void USART_TransmitDmaEnable() {
  // 送信完了割り込みの開始
  LL_DMA_EnableIT_TC(DMAx, TX_Stream);
  // USARTの開始
  LL_USART_Enable(USARTx);
}

/***
 * @brief  Start receiving data via DMA
 * @note   This function is executed when the microcontroller is powered on
 */
void USART_ReceiveDmaEnable() {
  // Initialize variables
  memset(kReadBuf, 0, RECV_BUFFER_SIZE);
  // Initialize read pointer
  kReadIndex = 0;
  // Initialize command processing buffer
//  ResetPacketStruct();
  // Disable DMA channel
  LL_DMA_DisableStream(DMAx, RX_Stream);
  // Configure DMA address
  LL_DMA_ConfigAddresses(
      DMAx, RX_Stream,
      LL_USART_DMA_GetRegAddr(USARTx, LL_USART_DMA_REG_DATA_RECEIVE),
      (uint32_t) kReadBuf,
      LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  // Set data length address
  LL_DMA_SetDataLength(DMAx, RX_Stream, RECV_BUFFER_SIZE);
  // Enable DMA channel
  LL_DMA_EnableStream(DMAx, RX_Stream);
  LL_USART_EnableDMAReq_RX(USARTx);
}

bool USART_IsTransmitFree() {
  return !kUsartBusy;
}

bool USART_PushWriteBuffer(uint8_t *buf, uint32_t length) {
  // データ数チェック
  if ((kWriteIndex + length) > SEND_BUFFER_SIZE) {
    return false;
  }

  // 送信バッファへ入力
  for (int i = 0; i < length; i++) {
    kSendBuf[i] = buf[i];
  }
  kWriteIndex += length;

  return true;
}

/**
 * @brief  USARTとDMAの送信設定と送信バッファに格納する関数
 */
void USART_TransmitStart() {
  // 送信状態へ遷移
  kUsartBusy = true;
  kTimeoutCnt = 0;

  // DMAチャネルの無効化
  LL_DMA_DisableStream(DMAx, TX_Stream);
  // 送信完了フラグのリセット
  LL_DMA_ClearFlag_TX(DMAx);
  // DMAアドレスの設定
  LL_DMA_ConfigAddresses(
      DMAx, TX_Stream, (uint32_t) kSendBuf,
      LL_USART_DMA_GetRegAddr(USARTx, LL_USART_DMA_REG_DATA_TRANSMIT),
      LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  // データ長の設定
  LL_DMA_SetDataLength(DMAx, TX_Stream, kWriteIndex);
  // DMAチャネルの有効
  LL_DMA_EnableStream(DMAx, TX_Stream);
  // DMA割り込みの有効
  LL_USART_EnableDMAReq_TX(USARTx);
  // USARTの送信完了割り込みの有効
  LL_USART_EnableIT_TC(USARTx);
}

void USART_AnyAddressTransmit(char *pointer, uint32_t length) {
  // 送信状態へ遷移
  kUsartBusy = true;
  kTimeoutCnt = 0;

  // DMAチャネルの無効化
  LL_DMA_DisableStream(DMAx, TX_Stream);
  // 送信完了フラグのリセット
  LL_DMA_ClearFlag_TX(DMAx);
  // DMAアドレスの設定
  LL_DMA_ConfigAddresses(
      DMAx, TX_Stream, (uint32_t) pointer,
      LL_USART_DMA_GetRegAddr(USARTx, LL_USART_DMA_REG_DATA_TRANSMIT),
      LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  // データ長の設定
  LL_DMA_SetDataLength(DMAx, TX_Stream, length);
  // DMAチャネルの有効
  LL_DMA_EnableStream(DMAx, TX_Stream);
  // DMA割り込みの有効
  LL_USART_EnableDMAReq_TX(USARTx);
  // USARTの送信完了割り込みの有効
  LL_USART_EnableIT_TC(USARTx);
}

void USART_ChangeBaudRate(uint32_t baud) {
  LL_USART_SetBaudRate(USARTx, GetUSARTxClockFreq,  //
                       LL_USART_OVERSAMPLING_16, baud);
}

void USART_TransmitBufferReset() {
  if (kWriteIndex > 0) {
    memset(kSendBuf, 0, sizeof(kSendBuf));
    kWriteIndex = 0;
  }
}

/**
 * @brief  USARTとDMAによる文字列の送信
 * @param[in]  buf : 文字列の先頭ポインタ
 * @return  送信の可否
 */
bool USART_Puts(char *buf) {
  // 送信バッファがリセットできたら送信する
  if (USART_IsTransmitFree()) {
    uint32_t length = strlen(buf);
    USART_TransmitBufferReset();
    USART_PushWriteBuffer((uint8_t*) buf, length);
    USART_TransmitStart();
    return true;
  } else {
    return false;
  }
}

/**
 * @brief  USARTとDMAによる指定データ数の送信
 * @param[in]  buf : 送信データの先頭ポインタ
 * @param[in]  length : データ長
 * @return  送信の可否
 */
bool USART_Putd(char *buf, uint32_t length) {
  // 送信がビジーでなければ送信開始
  if (USART_IsTransmitFree()) {
    USART_TransmitBufferReset();
    USART_PushWriteBuffer((uint8_t*) buf, length);
    USART_TransmitStart();
    return true;
  } else {
    return false;
  }
}

void USART_Putn(int32_t num, uint8_t len) {
  char *s = str_putn(num, len);
  USART_Puts(s);
}

void USART_Putx(uint32_t num, uint8_t len) {
  char *s = str_putx(num, len);
  USART_Puts(s);
}

void USART_Putf(float val, int bp, int ap) {
  char *s = str_putf(val, bp, ap);
  USART_Puts(s);
}

void USART_Putlf(float val, int ap) {
  char *s = str_putlf(val, ap);
  USART_Puts(s);
}

/**
 * @brief   DMAの送信状態のタイムアウトの監視
 */
void USART_BusyTimeout() {
  //送信状態の時判定
  if (kUsartBusy) {
    // 一定時間で送信完了割り込みが起こらなかったら強制的にリセットを行う
    if (kTimeoutCnt < USART_SEND_TIMOUT_TICK) {
      kTimeoutCnt++;
    } else {
      kTimeoutCnt = 0;
      kUsartBusy = false;
      LL_DMA_ClearFlag_TX(DMAx);
    }
  } else {
    kTimeoutCnt = 0;
  }
}

/**
 * @brief  USART受信バッファからの1文字受信
 * @return  char
 */
char USART_GetChar(void) {
  return USART_GetRingBuf();
}

/**
 * @brief  受信DMAのインデックス情報からread_bufの最新のデータのインデックスを取得する
 * @return  受信データのインデックス
 */
uint32_t USART_GetNewDataIndex() {
  return (RECV_BUFFER_SIZE - LL_DMA_NDTR) & (RECV_BUFFER_SIZE - 1);
}

/**
 * @brief  USARTを停止させる関数
 */
void USART_Abort() {
  CLEAR_BIT(
      USARTx->CR1,
      USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE);

  CLEAR_BIT(USARTx->CR3, USART_CR3_EIE);
  CLEAR_BIT(USARTx->CR3, USART_CR3_DMAT);

  // エラーフラグのリセット
  WRITE_REG(USARTx->ICR,
            USART_ICR_PECF|USART_ICR_FECF|USART_ICR_NCF | USART_ICR_ORECF);
}

/***
 * @brief  USARTのエラーチェック関数
 * @note  送信の直前にこの関数を置いてください。
 */
uint8_t USART_ErrorCheck() {
  //ORE,NE,FE,PEエラーフラグの監視
  if (USARTx->ISR & 0xF) {
    uint8_t ret = USARTx->ISR & 0xF;

    USART_Abort();
    USART_TransmitDmaEnable();
    USART_ReceiveDmaEnable();
    return ret;
  }
  return 0;
}

/**
 * @brief  新規受信データの数を返します。
 * @note  リングバッファのため未読数が最大サイズを超えると0に戻ります。
 * @return  uint32_t
 */
uint32_t USART_GetDataCount(void) {
  // 新規データのインデックス取得
  uint32_t new_index = USART_GetNewDataIndex();

  // 新規データindexが既読indexより上ななら減算処理
  if (new_index >= kReadIndex) {
    return (new_index - kReadIndex);
  }

  // 下ならリングバッファの上限値を考慮した加算処理
  return (RECV_BUFFER_SIZE - kReadIndex) + new_index;

}

/**
 * @brief  受信バッファから新規データを取得します
 * @return  char
 */
char USART_GetRingBuf(void) {
  if (USART_GetDataCount() != 0) {
    char c = kReadBuf[kReadIndex];
    kReadIndex = (kReadIndex + 1) % RECV_BUFFER_SIZE;
    return c;
  }
  return -1;
}

#endif
