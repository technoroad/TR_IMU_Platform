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

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
uint32_t kTable[256];

/* Global variables ----------------------------------------------------------*/

uint32_t MakeCrc32(uint16_t *data, uint32_t n) {
  uint32_t crc = 0xFFFFFFFF;
  uint32_t long_c;

  /* cycle through memory */
  for (int i = 0; i < n; i++) {
    /* Get lower byte */
    long_c = 0x000000ff & (uint32_t) data[i];
    /* Process with CRC */
    crc = ((crc >> 8) & 0x00ffffff) ^ kTable[(crc ^ long_c) & 0xff];
    /* Get upper byte */
    long_c = (0x000000ff & ((uint32_t) data[i] >> 8));
    /* Process with CRC */
    crc = ((crc >> 8) & 0x00ffffff) ^ kTable[(crc ^ long_c) & 0xff];
  }
  return crc;
}

void Crc32Initialize(void) {
  uint32_t P_32;
  uint32_t crc;

  /* CRC32 polynomial defined by IEEE-802.3 */
  P_32 = 0xEDB88320;
  /* 8 bits require 256 entries in Table */
  for (int i = 0; i < 256; i++) {
    /* start with table entry number */
    crc = (uint32_t) i;
    /* cycle through all bits in entry number */
    for (int j = 0; j < 8; j++) {
      /* LSBit set? */
      if ((crc & (uint32_t) 0x00000001) != (uint32_t) 0) {
        /* process for bit set */
        crc = (crc >> 1) ^ P_32;
      } else {
        /* process for bit clear */
        crc = (crc >> 1);
      }
    }
    /* Store calculated value into table */
    kTable[i] = crc;
  }
}
