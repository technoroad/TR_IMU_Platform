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

#ifndef APP_INC_SYSTEM_ERROR_H_
#define APP_INC_SYSTEM_ERROR_H_

#define E_STATUS 0x00
#define E_1647X 0x10
#define E_1649X 0x20
#define E_STM 0x30
#define E_OTHER 0x40
#define E_FATAL 0xE0

#define FATAL_ERROR_MASK E_FATAL

typedef enum {
  // status
  kNoneError = E_STATUS | 0x00,
  kUnknownError = E_STATUS | 0x01,

  // 1647x sensor error
  k1647xClockError = E_1647X | 0x00,
  k1647xMemoryError = E_1647X | 0x01,
  k1647xSensorFailure = E_1647X | 0x02,
  k1647xStandbyMode = E_1647X | 0x03,
  k1647xSpiCommError = E_1647X | 0x04,
  k1647xMemoryUpdateFailure = E_1647X | 0x05,
  k1647xDataPathOverrun = E_1647X | 0x06,

  // 1649x sensor error
  k1649xWatchdogTimer = E_1649X | 0x00,
  k1649xSyncError = E_1649X | 0x01,
  k1649xProcessingOverrun = E_1649X | 0x02,
  k1649xMemoryUpdateFailure = E_1649X | 0x03,
  k1649xSensorFailure = E_1649X | 0x04,
  k1649xSpiCommError = E_1649X | 0x05,
  k1649xSramError = E_1649X | 0x06,
  k1649xBootMemoryFailure = E_1649X | 0x07,

  // STM Error
  kIwdgResetDone = E_STM | 0x00,
  kTransmitBufferOverflow = E_STM | 0x01,
  kUsartHardError = E_STM | 0x02,
  kSpiTimeoutError = E_STM | 0x03,
  kPollingTimeoutError = E_STM | 0x04,
  kBurstReadChecksumError = E_STM | 0x05,
  kBurstReadCrc32Error = E_STM | 0x06,

  // Fatal error that halts the main program
  kIdIncorrectError = E_FATAL | 0x00,
} SystemError;

void SetSystemError(SystemError error);
SystemError GetSystemError();
void ResetSystemError();
bool IsFatalError(SystemError error);
SystemError DetectSensorError(uint16_t flag);

#endif /* APP_INC_SYSTEM_ERROR_H_ */
