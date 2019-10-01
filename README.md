# TR_IMU_Platform
“TR_IMU_Platform” is an Analog Devices IMU sensor that can be easily connected to ROS and output high-precision attitude angles.

(Sensor is sold separately)

Currently supported devices are:

- [ADIS16470/PCB](https://www.analog.com/en/design-center/evaluation-hardware-and-software/evaluation-boards-kits/EVAL-ADIS16470.html)
  - Wide Dynamic Range Mini MEMS IMU
  
- [ADSI16475-2/PCB](https://www.analog.com/en/design-center/evaluation-hardware-and-software/evaluation-boards-kits/EVAL-ADIS16475.html)
  - Precision, Miniature MEMs IMU
  
- [ADIS16495-2](https://www.analog.com/en/products/adis16495.html)
  - Higher grade MEMS IMU
  
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

# Firmware update
There are two types of firmware updates.

  - Write from USB(DFU)
    - When the 8th CONF switch is turned on and the power is turned on, the DFU mode is entered.
    - DFU can write firmware from [STM32CubeProg](https://www.st.com/en/development-tools/stm32cubeprog.html).
    - Download the sensor firmware [here](https://github.com/technoroad/TR_IMU_Platform/releases). 
  - Write or debug with ST-LINK
    - Writing and debugging are possible by connecting the MIPI-10 connector and ST-LINK.The following two are required.
      - [ST-LINK/V2](https://www.digikey.jp/product-detail/ja/stmicroelectronics/ST-LINK-V2/497-10484-ND/2214535)
      - [ARM-JTAG-20-10](https://www.digikey.jp/product-detail/ja/olimex-ltd/ARM-JTAG-20-10/1188-1016-ND/3471401)
    - [STM32CubeProg](https://www.st.com/en/development-tools/stm32cubeprog.html) or [ST-LINK Utility](https://www.st.com/en/development-tools/stsw-link004.html) can be used for writing.
    - When debugging, you need to import and build the source code after installing [TrueSTUDIO](https://www.st.com/ja/development-tools/truestudio.html).
