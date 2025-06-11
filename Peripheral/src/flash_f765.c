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
#define ERASE_FLASH_NUM  FLASH_SECTOR_1
// #define DOUBLEWORD_ONLY

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/**
 * @brief Function to clear flash memory
 *
 * This function clears the flash memory of the specified sector.
 *
 * @return A boolean value indicating whether the flash memory was successfully cleared.
 *         Returns true if successful, false otherwise.
 */
bool FLASH_Clear() {
  HAL_FLASH_Unlock();

  FLASH_EraseInitTypeDef EraseInitStruct;
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;  // Erase by page
  EraseInitStruct.Sector = ERASE_FLASH_NUM;       // Target page number to erase
  EraseInitStruct.NbSectors = 1;                        // Number of pages to erase
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3; // set voltage range (2.7 to 3.6V)
  EraseInitStruct.Banks = FLASH_BANK_1;               // Specify bank number (usually BANK 1)

  // The page number where an error occurred will be stored in error_page
  uint32_t error_page;
  HAL_StatusTypeDef result = HAL_FLASHEx_Erase(&EraseInitStruct, &error_page);

  HAL_FLASH_Lock();

  return result == HAL_OK && error_page == 0xFFFFFFFF;
}

/**
 * @brief Function to load data from flash memory
 *
 * This function reads data of the specified size from the given address.
 *
 * @param address The address to start reading from
 * @param data Pointer to the buffer where the read data will be stored
 * @param size Size of the data to read in bytes
 */
void FLASH_Load(uint32_t address, uint8_t *data, uint32_t size) {
  memcpy(data, (uint8_t*) address, size);
}

/**
 * @brief Function to write data to flash memory
 *
 * This function writes data of the specified size to the given address.
 * Whether the write was successful can be checked by the return value.
 *
 * @param address The address to start writing to
 * @param data Pointer to the buffer containing the data to write
 * @param size Size of the data to write in bytes
 * @param aligned Data alignment. Supported values: 1, 2, 4, or 8
 * @return Returns true if the write was successful, false otherwise
 */
bool FLASH_Store(uint32_t address, uint8_t *data, uint32_t size,
                 uint8_t aligned) {
  uint32_t type = 0;
  uint64_t val = 0;

#ifdef DOUBLEWORD_ONLY
  if(aligned != 8){
    // Do not execute if not aligned to doubleword
    return false;
  }

  // Fixed to doubleword
  type = FLASH_TYPEPROGRAM_DOUBLEWORD;
#else
  if (aligned == 1) {
    type = FLASH_TYPEPROGRAM_BYTE;
  } else if (aligned == 2) {
    type = FLASH_TYPEPROGRAM_HALFWORD;
  } else if (aligned == 4) {
    type = FLASH_TYPEPROGRAM_WORD;
  } else if (aligned == 8) {
    type = FLASH_TYPEPROGRAM_DOUBLEWORD;
  } else {
    return false;
  }
#endif

  // Clear flash
  if (!FLASH_Clear()) {
    return false;
  }

  // If the size is not divisible by the alignment, return false
  if ((size % aligned) != 0) {
    return false;
  }

  HAL_FLASH_Unlock();
  HAL_StatusTypeDef result;

  while (size) {
    if (aligned == 1) {
      val = *(uint8_t*) data;
    } else if (aligned == 2) {
      val = *(uint16_t*) data;
    } else if (aligned == 4) {
      val = *(uint32_t*) data;
    } else if (aligned == 8) {
      val = *(uint64_t*) data;
    }

    result = HAL_FLASH_Program(type, address, val);
    if (result != HAL_OK) {
      break;
    }

    address += aligned;
    data += aligned;
    size -= aligned;
  }

  HAL_FLASH_Lock();

  return result == HAL_OK;
}

#endif
