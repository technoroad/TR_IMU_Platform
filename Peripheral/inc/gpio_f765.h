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

#ifndef INC_GPIO_F765_H_
#define INC_GPIO_F765_H_

#if defined(STM32F765xx)
#define IMU_RESET_ON LL_GPIO_ResetOutputPin(ADIS_nRST_GPIO_Port, ADIS_nRST_Pin)
#define IMU_RESET_OFF LL_GPIO_SetOutputPin(ADIS_nRST_GPIO_Port, ADIS_nRST_Pin)
#define SPI_ENABLE_CHIPSELECT SPI4_nCS_GPIO_Port->BSRR = (SPI4_nCS_Pin << 16)
#define SPI_DISABLE_CHIPSELECT SPI4_nCS_GPIO_Port->BSRR = SPI4_nCS_Pin
#define RED_ON LL_GPIO_ResetOutputPin(LD1_GPIO_Port,LD1_Pin)
#define RED_OFF LL_GPIO_SetOutputPin(LD1_GPIO_Port,LD1_Pin)
#define GREEN_ON LL_GPIO_ResetOutputPin(LD2_GPIO_Port,LD2_Pin)
#define GREEN_OFF LL_GPIO_SetOutputPin(LD2_GPIO_Port,LD2_Pin)
#define BLUE_ON LL_GPIO_ResetOutputPin(LD3_GPIO_Port,LD3_Pin)
#define BLUE_OFF LL_GPIO_SetOutputPin(LD3_GPIO_Port,LD3_Pin)
#define DEBUG0_PIN_HIGH LL_GPIO_SetOutputPin(DEBUG_PIN0_GPIO_Port,DEBUG_PIN0_Pin)
#define DEBUG0_PIN_LOW LL_GPIO_ResetOutputPin(DEBUG_PIN0_GPIO_Port,DEBUG_PIN0_Pin)

#define READ_CSW1 !LL_GPIO_IsInputPinSet(CONF_SW1_GPIO_Port,CONF_SW1_Pin)
#define READ_CSW2 !LL_GPIO_IsInputPinSet(CONF_SW2_GPIO_Port,CONF_SW2_Pin)
#define READ_CSW3 !LL_GPIO_IsInputPinSet(CONF_SW3_GPIO_Port,CONF_SW3_Pin)
#define READ_CSW4 !LL_GPIO_IsInputPinSet(CONF_SW4_GPIO_Port,CONF_SW4_Pin)
#define READ_CSW5 !LL_GPIO_IsInputPinSet(CONF_SW5_GPIO_Port,CONF_SW5_Pin)
#define READ_CSW6 !LL_GPIO_IsInputPinSet(CONF_SW6_GPIO_Port,CONF_SW6_Pin)
#define READ_CSW7 !LL_GPIO_IsInputPinSet(CONF_SW7_GPIO_Port,CONF_SW7_Pin)
#define READ_CSW8 !LL_GPIO_IsInputPinSet(CONF_SW8_GPIO_Port,CONF_SW8_Pin)
#define READ_USER_SW !LL_GPIO_IsInputPinSet(USER_SW_GPIO_Port,USER_SW_Pin)
#endif

#endif /* INC_GPIO_F765_H_ */
