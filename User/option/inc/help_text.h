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


#ifndef OPTION_INC_HELP_TEXT_H_
#define OPTION_INC_HELP_TEXT_H_

const char *help_text =
    "start : Data serial communication output start\r\n"
    "  format : start[LF]\r\n"
    "\r\n"
    "stop : Data serial communication output stop\r\n"
    "  format : stop[LF]\r\n"
    "\r\n"
    "reset : The STM32 processor will be soft reset.\r\n"
    "  At this time, the COM port will be disconnected.\r\n"
    "  format : reset[LF]\r\n"
    "\r\n"
    "reinit : The system will reset with the power on.\r\n"
    "  At this time, the COM port is maintained.\r\n"
    "  Use ""RESET_FILTER"" if you want to reset pose estimation."
    "  format : reinit[LF]\r\n"
    "\r\n"
    "status : returns the status\r\n"
    "  This command is a modified command of GET_STATUS.\r\n"
    "  format : status[LF]\r\n"
    "  ret.1  : status,Ready\r\n"
    "    Ready to transmit IMU data\r\n"
    "  ret.2  : status,Transmitting\r\n"
    "    Transmitting IMU data\r\n"
    "  ret.3  : status,StartupWait\r\n"
    "    Waiting for ""Startup time""\r\n"
    // This message no longer exists as I removed the error status
/*    "  ret.4  : status,Error,error_code\r\n"
    "    State in which an error is detected and data output is stopped\r\n" */
    "\r\n"
    "error : return error code\r\n"
    "  format : error[LF]\r\n"
    "  Running this command will reset the error code\r\n"
    "\r\n"
    "bias : Reset the gyro bias value(Same as START_BIAS_CORRECTION)\r\n"
    "  format : bias[LF]\r\n"
    "\r\n"
    "filter : reset the pose estimation value to 0(Same as RESET_FILTER)\r\n"
    "  format : filter[LF]\r\n"
    "\r\n"
    "START_BIAS_CORRECTION : Reset the gyro bias value\r\n"
    "  format : START_BIAS_CORRECTION[LF]\r\n"
    "\r\n"
    "WRITE_REG : set a 2byte in any register\r\n"
    "  format : WRITE_REG,page,address,value[LF]\r\n"
    "  ex.    : WRITE_REG,0,0x0064,0x0013\r\n"
    "  Always set page to 0 for the ADIS1647X series.\r\n"
    "\r\n"
    "READ_REG : read a 2byte in any register\r\n"
    "  format : READ_REG,page,address[LF]\r\n"
    "  ex.    : READ_REG,0,0x0064\r\n"
    "  Always set page to 0 for the ADIS1647X series.\r\n"
    "\r\n"
    "START_BIAS_CORRECTION : Run ""bias correction""\r\n"
    "  format : START_BIAS_CORRECTION[LF]\r\n"
    "\r\n"
    "PAGE_DUMP : This command has been removed.\r\n"
    "\r\n"
    "HARD_FILTER_SELECT : select a filter\r\n"
    "  format : HARD_FILTER_SELECT,f0,f1,f2,f3,f4,f5[LF]\r\n"
    "  ex.    : HARD_FILTER_SELECT,2,0,0,0,0,0\r\n"
    "  Always set f1~f5 to 0 for the ADIS1647X series.\r\n"
    "  See FILT_CTRL in datasheet\r\n"
    "\r\n"
    "READ_TEMP : Read the temperature inside the sensor.\r\n"
    "  format : READ_TEMP[LF]\r\n"
    "\r\n"
    "SET_KP_KI : Set Kp and Ki of the filter.\r\n"
    "  format  : SET_KP_KI,Kp,Ki[LF]\r\n"
    "  ex.     : SET_KP_KI,1,0.1\r\n"
    "\r\n"
    "RESET_FILTER : reset the pose estimation value to 0\r\n"
    "  format :RESET_FILTER[LF]\r\n"
    "\r\n"
    "LOAD_INIT : Resets configurable values\r\n"
    "  format :LOAD_INIT[LF]\r\n"
    "  Run SAVE_PARAM to save\r\n"
    "\r\n"
    "SAVE_PARAM : Save settings\r\n"
    "  format : SAVE_PARAM[LF]\r\n"
    "\r\n"
    "SET_SEND_CYCLE : Set the transmission cycle of IMU data in units of [ms].\r\n"
    "  format : SET_SEND_CYCLE,cycle[LF]\r\n"
    "  ex.    : SET_SEND_CYCLE,1000\r\n"
    "  The TR-IMU1647X series can be set in increments of 10[ms].\r\n"
    "\r\n"
    "GET_VERSION : returns the firmware version\r\n"
    "  format : GET_VERSION[LF]\r\n"
    "\r\n"
    "GET_PROD_ID : Get the product id of the sensor\r\n"
    "  format : GET_PROD_ID[LF]\r\n"
    "\r\n"
    "GET_FORMAT : returns the format of the set output mode\r\n"
    "  format : GET_FORMAT[LF]\r\n"
    "  ret.1  : GET_FORMAT,YAW[deg],PITCH[deg],ROLL[deg]\r\n"
    "  ret.2  : GET_FORMAT,X_GYRO[deg/s],Y_GYRO[deg/s],Z_GYRO[deg/s]\r\n"
    "\r\n"
    "GET_SENSI : returns the sensitivity of the sensor\r\n"
    "  format : GET_SENSI[LF]\r\n"
    "  return : GET_SENSI,gyro_sensi,accl_sensi\r\n"
    "  The value can be obtained by multiplying this value with the digital data of the sensor.\r\n"
    "    ex.1 : gyro_32bit * gyro_sensi * PI * 180.0 = [deg/s]\r\n"
    "    ex.2 : accl_32bit * accl_sensi = [g]\r\n"
    "\r\n"
    "GET_BOARD_NAME : get the board name\r\n"
    "  format : GET_BOARD_NAME[LF]\r\n"
    "  ret.1  : GET_BOARD_NAME,TR-IMU1647X\r\n"
    "  ret.2  : GET_BOARD_NAME,TR-IMU-Platform\r\n"
    "\r\n"
    "GET_STATUS : returns the status\r\n"
    "  format : GET_STATUS[LF]\r\n"
    "  ret.1  : GET_STATUS,Ready\r\n"
    "    Waiting\r\n"
    "  ret.2  : GET_STATUS,Running\r\n"
    "    Outputting IMU data\r\n"
    "  ret.3  : GET_STATUS,AutoBiasUpdating\r\n"
    "    Waiting for ""Startup time""\r\n"
    // This message no longer exists as I removed the error status
/*    "  ret.4  : GET_STATUS,Error,error_code\r\n"
    "    State in which an error is detected and data output is stopped\r\n" */
    "\r\n"
    "SET_STARTUP_TIME : Sets the time to wait for the sensor to stabilize on startup\r\n"
    "  format : SET_STARTUP_TIME,second[LF]\r\n"
    "  Please input in seconds\r\n"
    "\r\n"
    "SET_FORMAT : Set the output mode with the command\r\n"
    "  format : SET_FORMAT,mode[LF]\r\n"
    "  mode is a value from 1 to 6, same value as toggle switch\r\n"
    "\r\n"

    "About errors.\r\n"
    "Errors from 01h to DFh are reported with the ""error"" command but do not stop the program.\r\n"
    "Errors from E0h to FFh stop IMU data acquisition and transmission.\r\n"
    "Error Code List:\r\n"
    "00h : no error\r\n"

    // 1647X Series hardware error
    "10h : 1647X clock error\r\n"
    "11h : 1647X memory error\r\n"
    "12h : 1647X sensor failure\r\n"
    "13h : 1647X standby sode\r\n"
    "14h : 1647X spi communication error\r\n"
    "15h : 1647X memory update failure\r\n"
    "16h : 1647X data path overrun\r\n"

    // 1649X Series hardware error
    "20h : 1649X watchdog timer\r\n"
    "21h : 1649X sync error\r\n"
    "22h : 1649X processing overrun\r\n"
    "23h : 1649X memory update failure\r\n"
    "24h : 1649X sensor failure\r\n"
    "25h : 1649X spi communication error\r\n"
    "26h : 1649X SRAM error\r\n"
    "27h : 1649X boot memory failure\r\n"

    // STM32 error
    "30h : STM32 iwdg reset detection\r\n"
    "31h : STM32 transmit buffer overflow error\r\n"
    "32h : STM32 usart hardware error\r\n"
    "33h : STM32 spi timeout error\r\n"
    "34h : STM32 polling transmit timeout error\r\n"
    "35h : STM32 burst read checksum error\r\n"
    "36h : STM32 burst read crc32 error(ADIS1649X only)\r\n"
//    "37h : STM32 DR pin collision error\r\n"

    "\r\n"
    "The program stops with the following error:\r\n"
    "E0h : Sensor model number is different\r\n"
  ;

#endif /* OPTION_INC_HELP_TEXT_H_ */
