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

// base file
#include "main.h"
#include "main_app.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"
#include "math.h"

// data defines
#include "defines.h"
#include "initial_parameter.h"
#include "select_sensor.h"

// peripheral
#if defined(STM32F070xB)
#include "board_f070.h"
#include "gpio_f070.h"
#include "exti_f070.h"
#include "flash_f070.h"
#include "iwdg_f070.h"
#include "spi_f070.h"
#include "usart_f070.h"
#elif defined(STM32F765xx)
#include "board_f765.h"
#include "gpio_f765.h"
#include "exti_f765.h"
#include "flash_f765.h"
#include "iwdg_f765.h"
#include "spi_f765.h"
#include "usart_f765.h"
#include "xport_f765.h"
#include "crc32.h"
#endif

// option
#include "delay.h"
#include "str_edit.h"
#include "csum.h"

// sensors
#include "adis_spi.h"
#include "adis_data.h"
#include "adis_ctrl.h"
#include "adis_error.h"

// filter
#include "mkae_filter.h"

// usb
#include "usbd_cdc_if.h"
#include "usbprint.h"

// communication
#include "usb_uart_select.h"
#include "binary_cmd_parser.h"
#include "string_cmd_parser.h"
#include "send_ack.h"

// management
#include "startup_wait.h"
#include "system_manager.h"
#include "system_error.h"

