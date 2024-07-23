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
#include "help_text.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define STR_BUF_SIZE 128
#define WORD_MAX 32
#define MSG_BUF_SIZE 128
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static uint32_t kStringLength = 0;
static char kStringBuf[STR_BUF_SIZE] = { 0 };
static uint32_t kWordLenth = 0;
static char *kWords[WORD_MAX];
static char kMsgBuf[MSG_BUF_SIZE] = { 0 };
static bool kEnableStringCommand = false;

/* Private function prototypes -----------------------------------------------*/
static bool START_func();
static bool STOP_func();
static bool REINIT_func();
static bool RESET_func();
static bool BIAS_func();
static bool STATUS_func();
static bool ERROR_func();
static bool FILTER_func();
static bool START_BIAS_CORRECTION_func();
static bool WRITE_REG_func();
static bool READ_REG_func();
static bool PAGE_DUMP_func();
static bool HARD_FILTER_SELECT_func();
static bool READ_TEMP_func();
static bool SET_KP_KI_func();
static bool RESET_FILTER_func();
static void other_func();

static bool LOAD_INIT_func();
static bool SAVE_PARAM_func();
static bool DUMP_PARAM_func();
static bool SET_SEND_CYCLE_func();
static bool GET_VERSION_func();
static bool GET_PROD_ID_func();
static bool GET_FORMAT_func();
static bool GET_SENSI_func();
static bool GET_BOARD_NAME_func();
static bool GET_STATUS_func();
static bool SET_STARTUP_TIME_func();
static bool SET_FORMAT_func();
static bool AUTO_UPDATE_ON_func();
static bool AUTO_UPDATE_OFF_func();
static bool SET_AUTO_UPDATE_TIME_func();
static bool GET_AUTO_UPDATE_TIME_func();
static bool STATIONAL_OBSERVE_ON_func();
static bool STATIONAL_OBSERVE_OFF_func();
static bool SET_THRESHOLD_ACCL_func();
static bool SET_THRESHOLD_GYRO_func();
static bool GET_THRESHOLD_ACCL_func();
static bool GET_THRESHOLD_GYRO_func();
static bool HELP_func();

/* Global variables ----------------------------------------------------------*/
extern BoardParameterList gBoard;

//Command parse processing
void StringCmdParse(char c) {
  // Allow only characters to be inserted into the buffer
  if ((c > 0x20) && (c < 0x7F)) {
    // The end of the buffer is reserved for a newline code
    if (kStringLength < (STR_BUF_SIZE - 1)) {
      kStringBuf[kStringLength++] = c;
    }
  } else {
    // Use newline code as end of sentence
    if (c == '\n') {
      kStringBuf[kStringLength] = '\0';
    }
  }

  // Start command parsing when line feed code is received
  if (c == '\n') {
    //Split string by ','
    kWordLenth = StringSplit(kStringBuf, kWords, ",", WORD_MAX);

    // Exit if there is no received character.
    if (kWordLenth == 0) {
      kWords[0] = "";
    }

    //Branch by header character
    if (strcmp(kWords[0], "start") == 0) {
      START_func();
    } else if (strcmp(kWords[0], "stop") == 0) {
      STOP_func();
    } else if (strcmp(kWords[0], "reset") == 0) {
      RESET_func();
    } else if (strcmp(kWords[0], "reinit") == 0) {
      REINIT_func();
    } else if (strcmp(kWords[0], "status") == 0) {
      STATUS_func();
    } else if (strcmp(kWords[0], "error") == 0) {
      ERROR_func();
    } else if (strcmp(kWords[0], "help") == 0) {
      HELP_func();
    } else if (strcmp(kWords[0], "bias") == 0) {
      BIAS_func();
    } else if (strcmp(kWords[0], "filter") == 0) {
      FILTER_func();
    } else if (strcmp(kWords[0], "START_BIAS_CORRECTION") == 0) {
      START_BIAS_CORRECTION_func();
    } else if (strcmp(kWords[0], "WRITE_REG") == 0) {
      if (!WRITE_REG_func()) {
        COMM_puts("ERROR_WRITE_REG\r\n");
      }
    } else if (strcmp(kWords[0], "READ_REG") == 0) {
      if (!READ_REG_func()) {
        COMM_puts("ERROR_READ_REG\r\n");
      }
    } else if (strcmp(kWords[0], "PAGE_DUMP") == 0) {
      if (!PAGE_DUMP_func()) {
        COMM_puts("ERROR_PAGE_DUMP\r\n");
      }
    } else if (strcmp(kWords[0], "HARD_FILTER_SELECT") == 0) {
      if (!HARD_FILTER_SELECT_func()) {
        COMM_puts("ERROR_HARD_FILTER_SELECT\r\n");
      }
    } else if (strcmp(kWords[0], "READ_TEMP") == 0) {
      if (!READ_TEMP_func()) {
        COMM_puts("ERROR_READ_TEMP\r\n");
      }
    } else if (strcmp(kWords[0], "SET_KP_KI") == 0) {
      if (!SET_KP_KI_func()) {
        COMM_puts("ERROR_SET_KP_KI\r\n");
      }
    } else if (strcmp(kWords[0], "RESET_FILTER") == 0) {
      if (!RESET_FILTER_func()) {
        COMM_puts("ERROR_RESET_FILTER\r\n");
      }
    } else if (strcmp(kWords[0], "LOAD_INIT") == 0) {
      if (!LOAD_INIT_func()) {
        COMM_puts("ERROR_LOAD_INIT\r\n");
      }
    } else if (strcmp(kWords[0], "SAVE_PARAM") == 0) {
      if (!SAVE_PARAM_func()) {
        COMM_puts("ERROR_SAVE_PARAM\r\n");
      }
    } else if (strcmp(kWords[0], "DUMP_PARAM") == 0) {
      if (!DUMP_PARAM_func()) {
        COMM_puts("ERROR_DUMP_PARAM\r\n");
      }
    } else if (strcmp(kWords[0], "GET_VERSION") == 0) {
      if (!GET_VERSION_func()) {
        COMM_puts("ERROR_GET_VERSION\r\n");
      }
    } else if (strcmp(kWords[0], "SET_SEND_CYCLE") == 0) {
      if (!SET_SEND_CYCLE_func()) {
        COMM_puts("ERROR_SET_SEND_CYCLE\r\n");
      }
    } else if (strcmp(kWords[0], "GET_PROD_ID") == 0) {
      if (!GET_PROD_ID_func()) {
        COMM_puts("ERROR_GET_PROD_ID\r\n");
      }
    } else if (strcmp(kWords[0], "GET_FORMAT") == 0) {
      if (!GET_FORMAT_func()) {
        COMM_puts("ERROR_GET_FORMAT\r\n");
      }
    } else if (strcmp(kWords[0], "GET_SENSI") == 0) {
      if (!GET_SENSI_func()) {
        COMM_puts("ERROR_GET_SENSI\r\n");
      }
    } else if (strcmp(kWords[0], "GET_BOARD_NAME") == 0) {
      if (!GET_BOARD_NAME_func()) {
        COMM_puts("ERROR_GET_BOARD_NAME\r\n");
      }
    } else if (strcmp(kWords[0], "GET_STATUS") == 0) {
      if (!GET_STATUS_func()) {
        COMM_puts("ERROR_STATUS\r\n");
      }
    } else if (strcmp(kWords[0], "SET_STARTUP_TIME") == 0) {
      if (!SET_STARTUP_TIME_func()) {
        COMM_puts("ERROR_STARTUP_TIME\r\n");
      }
    } else if (strcmp(kWords[0], "SET_FORMAT") == 0) {
      if (!SET_FORMAT_func()) {
        COMM_puts("ERROR_SET_FORMAT\r\n");
      }
    } else if (strcmp(kWords[0], "AUTO_UPDATE_ON") == 0) {
      if (!AUTO_UPDATE_ON_func()) {
        COMM_puts("ERROR_AUTO_UPDATE_ON\r\n");
      }
    } else if (strcmp(kWords[0], "AUTO_UPDATE_OFF") == 0) {
      if (!AUTO_UPDATE_OFF_func()) {
        COMM_puts("ERROR_AUTO_UPDATE_OFF\r\n");
      }
    } else if (strcmp(kWords[0], "SET_AUTO_UPDATE_TIME") == 0) {
      if (!SET_AUTO_UPDATE_TIME_func()) {
        COMM_puts("ERROR_SET_AUTO_UPDATE_TIME\r\n");
      }
    } else if (strcmp(kWords[0], "GET_AUTO_UPDATE_TIME") == 0) {
      if (!GET_AUTO_UPDATE_TIME_func()) {
        COMM_puts("ERROR_GET_AUTO_UPDATE_TIME\r\n");
      }
    } else if (strcmp(kWords[0], "STATIONAL_OBSERVE_ON") == 0) {
      if (!STATIONAL_OBSERVE_ON_func()) {
        COMM_puts("ERROR_STATIONAL_OBSERVE_ON\r\n");
      }
    } else if (strcmp(kWords[0], "STATIONAL_OBSERVE_OFF") == 0) {
      if (!STATIONAL_OBSERVE_OFF_func()) {
        COMM_puts("ERROR_STATIONAL_OBSERVE_OFF\r\n");
      }
    } else if (strcmp(kWords[0], "SET_THRESHOLD_ACCL") == 0) {
      if (!SET_THRESHOLD_ACCL_func()) {
        COMM_puts("ERROR_SET_THRESHOLD_ACCL\r\n");
      }
    } else if (strcmp(kWords[0], "SET_THRESHOLD_GYRO") == 0) {
      if (!SET_THRESHOLD_GYRO_func()) {
        COMM_puts("ERROR_SET_THRESHOLD_GYRO\r\n");
      }
    } else if (strcmp(kWords[0], "GET_THRESHOLD_ACCL") == 0) {
      if (!GET_THRESHOLD_ACCL_func()) {
        COMM_puts("ERROR_GET_THRESHOLD_ACCL\r\n");
      }
    } else if (strcmp(kWords[0], "GET_THRESHOLD_GYRO") == 0) {
      if (!GET_THRESHOLD_GYRO_func()) {
        COMM_puts("ERROR_GET_THRESHOLD_GYRO\r\n");
      }
    } else if (strcmp(kWords[0], "") == 0) {
    } else {
      other_func();
    }

    // Initialize
    kStringLength = 0;
    kWordLenth = 0;
    memset(kStringBuf, 0, sizeof(kStringBuf));
    memset(kMsgBuf, 0, sizeof(kMsgBuf));
  }
}

// Process received data as a character string.
void ReadStringCmd() {
  // If received, it is stored in the buffer.
  while (COMM_GetDataCount() != 0) {
    // Get received character from ring buffer.
    char buf = COMM_GetChar();

    // Command parse
    StringCmdParse(buf);
  }
}

bool START_func() {
  SendStart();
  COMM_puts("start\r\n");
  return true;
}

bool STOP_func() {
  SendStop();
  COMM_puts("stop\r\n");
  return true;
}

bool REINIT_func() {
  SendStop();
  COMM_PollingSend("reinit\r\n");
  ResetSystemStatus();
  return true;
}

bool RESET_func() {
  SendStop();
  COMM_PollingSend("reset\r\n");
  HAL_NVIC_SystemReset();
  return true;
}

bool BIAS_func() {
  IMU_BiasCorrectionUpdate();
  IMU_UpdateGyroBias();
  COMM_puts("bias\r\n");
  return true;
}

bool FILTER_func() {
  MKAE_MS2016_FilterReset();

  ImuDataList *list = IMU_GetDataList();
  list->yaw = 0;
  list->pitch = 0;
  list->roll = 0;

  COMM_puts("filter\r\n");

  return true;
}

bool START_BIAS_CORRECTION_func() {
  IMU_BiasCorrectionUpdate();
  IMU_UpdateGyroBias();
  COMM_puts("START_BIAS_CORRECTION\r\n");
  return true;
}

bool WRITE_REG_func() {
  if (kWordLenth != 4)
    return false;
  if (!IsHexString(kWords[1]))
    return false;
  if (!IsHexString(kWords[2]))
    return false;
  if (!IsHexString(kWords[3]))
    return false;

  uint16_t page = HexStringToU16(kWords[1]);
  uint16_t addr = HexStringToU16(kWords[2]);
  uint16_t value = HexStringToU16(kWords[3]);

  IMU_WriteData(page, addr, value);

  str_concat(kMsgBuf, "WRITE_REG,");
  str_concat(kMsgBuf, str_putx(page, 2));
  str_concat(kMsgBuf, ",");
  str_concat(kMsgBuf, str_putx(addr, 2));
  str_concat(kMsgBuf, ",");
  str_concat(kMsgBuf, str_putx(value, 4));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool READ_REG_func() {
  if (kWordLenth != 3)
    return false;
  if (!IsHexString(kWords[1]))
    return false;
  if (!IsHexString(kWords[2]))
    return false;

  uint16_t page = HexStringToU16(kWords[1]);
  uint16_t addr = HexStringToU16(kWords[2]);

  if (IMU_IsBurstReadCommand(page, addr)) {
    return false;
  }

  uint16_t value = IMU_ReadData(page, addr);
  str_concat(kMsgBuf, "READ_REG,");
  str_concat(kMsgBuf, str_putx(page, 2));
  str_concat(kMsgBuf, ",");
  str_concat(kMsgBuf, str_putx(addr, 2));
  str_concat(kMsgBuf, ",");
  str_concat(kMsgBuf, str_putx(value, 4));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool PAGE_DUMP_func() {
#if 0
  if (kWordLenth != 2)
    return false;
  if (!IsHexString(kWords[1]))
    return false;

  uint16_t page = HexStringToU16(kWords[1]);
  uint16_t data[64] = { 0 };

  SendStop();

  uint16_t page_len = IMU_ReadAllPageData(page, data);

  char buf[1024] = { 0 };

  str_concat(buf, "PAGE_DUMP,");
  str_concat(buf, str_putx(page, 2));
  str_concat(buf, ",");

  for (int i = 0; i < page_len; i++) {
    str_concat(buf, str_putx(data[i], 4));
    if (i < (page_len - 1)) {
      str_concat(buf, ",");
    }
  }
  str_concat(buf, "\r\n");
  COMM_PollingSend((char*) buf);

  return true;
#else
  return false;
#endif
}

bool HARD_FILTER_SELECT_func() {
  if (kWordLenth != 7)
    return false;

  int array[6];
  for (int i = 0; i < 6; i++) {
    if (!IsDecString(kWords[i + 1]))
      return false;
    array[i] = atoi(kWords[i + 1]);
  }

  IMU_SetHardwareFilter(array[0], array[1],  //
                        array[2], array[3], array[4], array[5]);

  str_concat(kMsgBuf, "HARD_FILTER_SELECT,");
  for (int i = 0; i < 6; i++) {
    str_concat(kMsgBuf, str_putn(array[i], 2));
  }
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool READ_TEMP_func() {
  ImuDataList *list = IMU_GetDataList();

  str_concat(kMsgBuf, "READ_TEMP,");
  str_concat(kMsgBuf, str_putlf(list->temperature, 5));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool SET_KP_KI_func() {
#ifdef MASUYA
  if (kWordLenth != 3)
    return false;

  if (!IsDoubleString(kWords[1]))
    return false;
  if (!IsDoubleString(kWords[2]))
    return false;

  gBoard.gain_p = DoubleStringToDouble(kWords[1]);
  gBoard.gain_i = DoubleStringToDouble(kWords[2]);

  str_concat(kMsgBuf, "SET_KP_KI,");
  str_concat(kMsgBuf, str_putlf(gBoard.gain_p, 5));
  str_concat(kMsgBuf, ",");
  str_concat(kMsgBuf, str_putlf(gBoard.gain_i, 5));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);
#endif

  return true;
}

bool RESET_FILTER_func() {
  MKAE_MS2016_FilterReset();

  str_concat(kMsgBuf, "RESET_FILTER\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

static bool LOAD_INIT_func() {
  InitializeParameters();

  str_concat(kMsgBuf, "LOAD_INIT\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

static bool SAVE_PARAM_func() {
  SendStop();

  uint32_t size = sizeof(gBoard);
  gBoard.csum = Make16bitChecksum((uint8_t*) &gBoard, size - CSUM_SIZE);
  WriteFlash(DATA_ADDR, (uint16_t*) &gBoard, size);

  str_concat(kMsgBuf, "SAVE_PARAM\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

static bool DUMP_PARAM_func() {
  SendStop();

  double cycle = (gBoard.transmit_prescaler + 1) * CTRL_PERIOD * 1000.0;

  str_concat(kMsgBuf, "DUMP_PARAM,");
  str_concat(kMsgBuf, str_putx(gBoard.version, 8));
  str_concat(kMsgBuf, ",");
  str_concat(kMsgBuf, str_putlf(gBoard.gain_p, 5));
  str_concat(kMsgBuf, ",");
  str_concat(kMsgBuf, str_putlf(gBoard.gain_i, 5));
  str_concat(kMsgBuf, ",");
  str_concat(kMsgBuf, str_putn2(cycle));
  str_concat(kMsgBuf, ",");
  str_concat(kMsgBuf, str_putn2(gBoard.startup_time));

  str_concat(kMsgBuf, "\r\n");
  COMM_PollingSend(kMsgBuf);

  return true;
}

static bool GET_VERSION_func() {
  str_concat(kMsgBuf, "GET_VERSION,");
  str_concat(kMsgBuf, str_putx(gBoard.version, 8));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

static bool SET_SEND_CYCLE_func() {
  if (kWordLenth != 2)
    return false;

  if (!IsDecString(kWords[1]))
    return false;

  uint16_t cycle = DecStringToDec(kWords[1]);
  uint16_t min_cycle = (uint16_t) (CTRL_PERIOD * 1000.0);

  cycle = cycle - cycle % min_cycle;

  if (cycle < min_cycle) {
    return false;
  }

  gBoard.transmit_prescaler = (cycle / min_cycle) - 1;

  double ssc = (gBoard.transmit_prescaler + 1) * CTRL_PERIOD * 1000.0;

  str_concat(kMsgBuf, "SET_SEND_CYCLE,");
  str_concat(kMsgBuf, str_putn2(ssc));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

static bool GET_PROD_ID_func() {
  str_concat(kMsgBuf, "GET_PROD_ID,ADIS");
  str_concat(kMsgBuf, str_putn2(PRODUCT_ID));

#if defined(_1BMLZ)
  str_concat(kMsgBuf,"-1");
#elif defined(_2BMLZ)
  str_concat(kMsgBuf, "-2");
#elif defined(_3BMLZ)
  str_concat(kMsgBuf,"-3");
#elif defined(NONE_BMLZ)
#endif

  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

static bool GET_FORMAT_func() {
  StringFormat format = GetOutputFormat();

  str_concat(kMsgBuf, "GET_FORMAT,");
  switch (format) {
    case kYawPitchRoll:
      str_concat(kMsgBuf, "YAW[deg],PITCH[deg],ROLL[deg]");
      break;
    case kGYRO_Degree:
      str_concat(kMsgBuf, "X_GYRO[deg/s],Y_GYRO[deg/s],Z_GYRO[deg/s]");
      break;
    case kGYRO_ACC_HexData:
      str_concat(kMsgBuf, "X_GYRO_HEX,Y_GYRO_HEX,Z_GYRO_HEX,"
                 "X_ACC_HEX,Y_ACC_HEX,Z_ACC_HEX,CSUM");
      break;
    case kGYRO_ACC_BinaryData:
      str_concat(kMsgBuf, "Binary");
      break;
    case kYawPitchRoll_ACC:
      str_concat(kMsgBuf,
                 "YAW[deg],PITCH[deg],ROLL[deg],X_ACC[g],Y_ACC[g],Z_ACC[g]");
      break;
    case kGYRO_ACC_TEMP:
      str_concat(
          kMsgBuf,
          "X_GYRO[deg/s],Y_GYRO[deg/s],Z_GYRO[deg/s],X_ACC[g],Y_ACC[g],Z_ACC[g],TEMP[deg]");
      break;
    case kPose_GYRO_ACC_CNT:
      str_concat(
          kMsgBuf,
          "YAW[deg],PITCH[deg],ROLL[deg],"
          "X_GYRO[deg/s],Y_GYRO[deg/s],Z_GYRO[deg/s],"
          "X_ACC[g],Y_ACC[g],Z_ACC[g],COUNT");
      break;
    default:
      return false;
      break;
  }

  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

static bool GET_SENSI_func() {
  double gyro_sensi, acc_sensi;
  IMU_GetSensitivity(&gyro_sensi, &acc_sensi);

  str_concat(kMsgBuf, "GET_SENSI,");
  str_concat(kMsgBuf, str_putlf(gyro_sensi, 1));
  str_concat(kMsgBuf, ",");
  str_concat(kMsgBuf, str_putlf(acc_sensi, 1));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

static bool GET_BOARD_NAME_func() {
  str_concat(kMsgBuf, "GET_BOARD_NAME,");
  str_concat(kMsgBuf, BoardName);
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

static bool GET_STATUS_func() {
  str_concat(kMsgBuf, "GET_STATUS,");

  if (IsStartupWaitEnable()) {
    str_concat(kMsgBuf, "AutoBiasUpdating,");
    str_concat(kMsgBuf, str_putn2(GetStartupWaitTimeLeft()));
  } else {
    if (IsSendEnable()) {
      str_concat(kMsgBuf, "Running");
    } else {
      str_concat(kMsgBuf, "Ready");
    }
  }

  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

static bool SET_STARTUP_TIME_func() {
  if (!IsDecString(kWords[1]))
    return false;

  uint16_t startup_time = DecStringToDec(kWords[1]);
  gBoard.startup_time = startup_time;

  str_concat(kMsgBuf, "SET_STARTUP_TIME,");
  str_concat(kMsgBuf, str_putn2(gBoard.startup_time));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool SET_FORMAT_func() {
  if (!IsDecString(kWords[1]))
    return false;

  StringFormat format = DecStringToDec(kWords[1]);

  str_concat(kMsgBuf, "SET_FORMAT,");

  switch (format) {
    case kYawPitchRoll:
      str_concat(kMsgBuf, "YAW[deg],PITCH[deg],ROLL[deg]");
      break;
    case kGYRO_Degree:
      str_concat(kMsgBuf, "X_GYRO[deg/s],Y_GYRO[deg/s],Z_GYRO[deg/s]");
      break;
    case kGYRO_ACC_HexData:
      str_concat(kMsgBuf, "X_GYRO_HEX,Y_GYRO_HEX,Z_GYRO_HEX,"
                 "X_ACC_HEX,Y_ACC_HEX,Z_ACC_HEX,CSUM");
      break;
    case kGYRO_ACC_BinaryData:
      str_concat(kMsgBuf, "Binary");
      break;
    case kYawPitchRoll_ACC:
      str_concat(kMsgBuf,
                 "YAW[deg],PITCH[deg],ROLL[deg],X_ACC[g],Y_ACC[g],Z_ACC[g]");
      break;
    case kGYRO_ACC_TEMP:
      str_concat(
          kMsgBuf,
          "X_GYRO[deg/s],Y_GYRO[deg/s],Z_GYRO[deg/s],X_ACC[g],Y_ACC[g],Z_ACC[g],TEMP[deg]");
      break;
    case kPose_GYRO_ACC_CNT:
      str_concat(
          kMsgBuf,
          "YAW[deg],PITCH[deg],ROLL[deg],"
          "X_GYRO[deg/s],Y_GYRO[deg/s],Z_GYRO[deg/s],"
          "X_ACC[g],Y_ACC[g],Z_ACC[g],COUNT");
      break;
    default:
      return false;
      break;
  }

  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  SetOutputFormat(format);

  return true;
}

bool AUTO_UPDATE_ON_func() {
  if (kWordLenth != 1)
    return false;

  gBoard.auto_update_enable = true;
  IMU_SetBiasCorrectionTime(gBoard.auto_update_time);

  str_concat(kMsgBuf, "AUTO_UPDATE_ON\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool AUTO_UPDATE_OFF_func() {
  if (kWordLenth != 1)
    return false;

  gBoard.auto_update_enable = false;

  str_concat(kMsgBuf, "AUTO_UPDATE_OFF\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool STATIONAL_OBSERVE_ON_func(){
  if (kWordLenth != 1)
    return false;

  gBoard.stationary_observe_enable = true;

  str_concat(kMsgBuf, "STATIONAL_OBSERVE_ON\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool SET_AUTO_UPDATE_TIME_func(){
  if (kWordLenth != 2)
    return false;

  if (!IsDecString(kWords[1]))
    return false;

  uint16_t auto_update_time = DecStringToDec(kWords[1]);
  gBoard.auto_update_time = auto_update_time;

  str_concat(kMsgBuf, "SET_AUTO_UPDATE_TIME,");
  str_concat(kMsgBuf, str_putn2(gBoard.auto_update_time));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool GET_AUTO_UPDATE_TIME_func(){
  str_concat(kMsgBuf, "GET_AUTO_UPDATE_TIME,");
  str_concat(kMsgBuf, str_putn2(gBoard.auto_update_time));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool STATIONAL_OBSERVE_OFF_func(){
  if (kWordLenth != 1)
    return false;

  gBoard.stationary_observe_enable = false;

  str_concat(kMsgBuf, "STATIONAL_OBSERVE_OFF\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool SET_THRESHOLD_ACCL_func() {
  if (kWordLenth != 2)
    return false;

  if (!IsDoubleString(kWords[1]))
    return false;

  gBoard.stational_accl_thresh = DoubleStringToDouble(kWords[1]);

  str_concat(kMsgBuf, "SET_THRESHOLD_ACCL,");
  str_concat(kMsgBuf, str_putlf(gBoard.stational_accl_thresh, 5));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool SET_THRESHOLD_GYRO_func() {
  if (kWordLenth != 2)
    return false;

  if (!IsDoubleString(kWords[1]))
    return false;

  gBoard.stational_gyro_thresh = DoubleStringToDouble(kWords[1]);

  str_concat(kMsgBuf, "SET_THRESHOLD_GYRO,");
  str_concat(kMsgBuf, str_putlf(gBoard.stational_gyro_thresh, 5));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool GET_THRESHOLD_ACCL_func(){
  str_concat(kMsgBuf, "GET_THRESHOLD_ACCL,");
  str_concat(kMsgBuf, str_putlf(gBoard.stational_accl_thresh, 3));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool GET_THRESHOLD_GYRO_func(){
  str_concat(kMsgBuf, "GET_THRESHOLD_GYRO,");
  str_concat(kMsgBuf, str_putlf(gBoard.stational_gyro_thresh, 3));
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}


bool HELP_func() {
  SendStop();
  COMM_PollingSend((char*) help_text);
  return true;
}

bool STATUS_func() {
  str_concat(kMsgBuf, "status,");

  if (IsStartupWaitEnable()) {
    str_concat(kMsgBuf, "StartupWait,");
    str_concat(kMsgBuf, str_putn2(GetStartupWaitTimeLeft()));
  } else {
    if (IsSendEnable()) {
      str_concat(kMsgBuf, "Transmitting");
    } else {
      str_concat(kMsgBuf, "Ready");
    }
  }

  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);

  return true;
}

bool ERROR_func() {
  SystemError error = GetSystemError();

  str_concat(kMsgBuf, "error,");
  str_concat(kMsgBuf, str_putx(error, 2));
  str_concat(kMsgBuf, "\r\n");

  if (IsFatalError(error)) {
    SendStop();
    COMM_PollingSend(kMsgBuf);
    ResetSystemError();
  } else {
    COMM_puts(kMsgBuf);
    ResetSystemError();
  }
  return true;
}

void other_func() {
  str_concat(kMsgBuf, "ERROR_NONE_CMD,");
  *(kWords[0] + 5) = '\0';  // character limit
  str_concat(kMsgBuf, kWords[0]);
  str_concat(kMsgBuf, "\r\n");
  COMM_puts(kMsgBuf);
}

/**
 * @brief  String Command mode is enabled.
 */
void EnableStringCommand() {
  kEnableStringCommand = true;
}

/**
 * @brief  String Command mode is disabled.
 */
void DisableStringCommand() {
  kEnableStringCommand = false;
}

/**
 * @brief  Returns whether String Command mode is enabled.
 */
bool IsEnableStringCommand() {
  return kEnableStringCommand;
}
