[![Latest Release](https://img.shields.io/github/v/release/technoroad/TR_IMU_Platform?include_prereleases&label=Latest%20Release)](https://github.com/technoroad/TR_IMU_Platform/releases/latest)
# TR_IMU_Platform
“TR_IMU_Platform” is an Analog Devices IMU sensor that can be easily connected to ROS and output high-precision attitude angles.

(Sensor is sold separately)

Currently supported devices are:

- Precision, Miniature MEMs IMU

  - [ADIS16470/PCB](https://www.analog.com/en/design-center/evaluation-hardware-and-software/evaluation-boards-kits/EVAL-ADIS16470.html)
  - [ADSI16475-2/PCB](https://www.analog.com/en/design-center/evaluation-hardware-and-software/evaluation-boards-kits/EVAL-ADIS16475.html)
  - [ADSI16477-2/PCB](https://www.analog.com/en/design-center/evaluation-hardware-and-software/evaluation-boards-kits/EVAL-ADIS16477.html)
  - [ADSI16500/PCB](https://www.analog.com/en/design-center/evaluation-hardware-and-software/evaluation-boards-kits/EVAL-ADIS16500.html)
  - [ADSI16505-2/PCB](https://www.analog.com/en/design-center/evaluation-hardware-and-software/evaluation-boards-kits/EVAL-ADIS16505.html)
  - [ADIS16IMU5/PCBZ](https://www.analog.com/jp/resources/evaluation-hardware-and-software/evaluation-boards-kits/eval-adis16imu5.html#eb-overview)
    - Mount this [ADIS16575-2](https://www.analog.com/jp/products/adis16575.html)
  
- Higher grade MEMS IMU
  - [ADIS16495-2](https://www.analog.com/en/products/adis16495.html)

The default is dedicated to ADIS16495-2, so the firmware needs to be updated if another sensor is used.

Support for other sensors is possible by adding a library of sensors.

# SPI Mode
It can operate as an alternative to USB-IIS introduced in [adi_driver](https://github.com/tork-a/adi_driver).

# Attitude angle mode
Since the attitude angle is calculated by the MCU, it is not necessary for the host to calculate it.
In addition, a nonlinear complementary filter can be used to obtain more accurate angles than those calculated by other systems.

When using this mode, it is recommended to operate with [IMU_PlatformTool](https://github.com/technoroad/IMU_PlatformTool).

# ADIS16470/PCB and ADSI16475-2/PCB
ADIS1647X / PCB series can be installed by using optional parts.
<div align="center">
  <img src="https://github.com/technoroad/TR_IMU_Platform/blob/images/img/ADIS16475-2.jpg" width="60%"/>
</div>

# ADIS16495-2

<div align="center">
  <img src="https://github.com/technoroad/TR_IMU_Platform/blob/images/img/ADIS16495.jpg" width="60%"/>
</div>

# New Features (Introduced in v1.5.0)
  - Starting from v1.5.0, we have merged the following two new features into the "main" branch:
    - Monitoring the accelerometer and gyro to update the gyro bias when the system is stationary.(excluding ADIS165XX series)
    - To prevent drift in the attitude angle along the vertical axis relative to gravity, monitoring the gyroscope and avoiding attitude angle calculations when the system is completely stationary.(excluding ADIS165XX series)
  - These features have not yet been thoroughly tested on various robots, so any feedback or information you can provide would be greatly appreciated.
  - The above features are OFF by default. To enable them, please issue the following command.
  - The pose estimation library has been updated to remove the functionality for calculating and removing the gyro bias.
~~~
AUTO_UPDATE_ON
STATIONAL_OBSERVE_ON
SAVE_PARAM
~~~
  - To view the command description, please enter "help" below.
~~~
help
~~~

# Firmware update
There are two types of firmware updates.

  - Write from USB(DFU)
    - When the 8th CONF switch is turned on and the power is turned on, the DFU mode is entered.
    - DFU can write firmware from [STM32CubeProg](https://www.st.com/en/development-tools/stm32cubeprog.html).
    - Download the sensor firmware [here](https://github.com/technoroad/TR_IMU_Platform/releases/latest). 
  - Write or debug with ST-LINK
    - Writing and debugging are possible by connecting the MIPI-10 connector and ST-LINK.The following two are required.
      - [STLINK-V3SET](https://www.digikey.jp/ja/products/detail/stmicroelectronics/STLINK-V3SET/9636028)
        - V3SET can be directly connected to [TR-IMU1647X](https://github.com/TR-Hiroki-Kobayashi/TR-IMU1647X) or [TR_IMU_Platform](https://github.com/technoroad/TR_IMU_Platform).
      - [ST-LINK/V2](https://www.digikey.jp/product-detail/ja/stmicroelectronics/ST-LINK-V2/497-10484-ND/2214535)
        - Requires [ARM-JTAG-20-10](https://www.digikey.jp/product-detail/ja/olimex-ltd/ARM-JTAG-20-10/1188-1016-ND/3471401) for V2
    - [STM32CubeProg](https://www.st.com/en/development-tools/stm32cubeprog.html) or [ST-LINK Utility](https://www.st.com/en/development-tools/stsw-link004.html) can be used for writing.
    - When debugging, you need to import and build the source code after installing [STM32CubeIDE](https://www.st.com/ja/development-tools/stm32cubeide.html) ~~[TrueSTUDIO]~~.
# Latest Release

[![Latest Release](https://img.shields.io/github/v/release/technoroad/TR_IMU_Platform?include_prereleases&label=Latest%20Release)](https://github.com/technoroad/TR_IMU_Platform/releases/latest)

You can access the latest release from [here](https://github.com/technoroad/TR_IMU_Platform/releases). For more details and changelog, please refer to the [Release Notes](https://github.com/technoroad/TR_IMU_Platform/releases).
