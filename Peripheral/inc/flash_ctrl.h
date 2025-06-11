/*
 * flash_ctrl.h
 *
 *  Created on: May 7, 2024
 *      Author: techno-road
 */

#ifndef HARDWARE_INC_FLASH_CTRL_H_
#define HARDWARE_INC_FLASH_CTRL_H_

#include "libraries.h"

void LoadSettingsFromFlash(bool init_flg);
bool SaveSettingsToFlash();
void SetInitializeSettingsToStruct();
Flash_Settings* GetFlashSettingPtr();
void SetFlashImuSetting(IMU_Config *config);

#endif /* HARDWARE_INC_FLASH_CTRL_H_ */
