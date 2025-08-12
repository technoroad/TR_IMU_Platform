/*
 * pwm_f765.c
 *
 *  Created on: 2025/08/12
 *      Author: techno-road
 */

/* Includes ------------------------------------------------------------------*/
#include "libraries.h"

/* Global variables ----------------------------------------------------------*/
extern System_Status gSystem;

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define TIMx TIM10

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


void PWM_Initialize(){
#if defined(STM32F070xB)
#elif defined(STM32F765xx)
  uint16_t pres = 0;
  uint16_t arr = 0;

  switch(gSystem.tset.conversion_rate){
    case 2000:{
      pres = F7_2KHZ_PWM_PRES;
      arr = F7_2KHZ_PWM_ARR;
      break;
    }
    case 4000:{
      pres = F7_4KHZ_PWM_PRES;
      arr = F7_4KHZ_PWM_ARR;
      break;
    }
    case 9000:{
      pres = F7_9KHZ_PWM_PRES;
      arr = F7_9KHZ_PWM_ARR;
      break;
    }
    default :{
      pres = F7_2KHZ_PWM_PRES;
      arr = F7_2KHZ_PWM_ARR;
      break;
    }
  }

  LL_TIM_SetPrescaler(TIMx,pres);
  LL_TIM_SetAutoReload(TIMx, arr);
  LL_TIM_OC_SetCompareCH1(TIMx, arr / 2);
#endif
}

void PWM_StartOutput(){
  LL_TIM_CC_EnableChannel(TIMx,LL_TIM_CHANNEL_CH1);
  LL_TIM_EnableCounter(TIMx);
}
