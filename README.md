# PS3-Arducopter
Arduino based Quadcopter Drone that is wirelessly controlled by a Playstation 3 Controller

The goal of this project is to design, program and build an Arduino based Quadcopter Drone that is wirelessly controlled by a Playstation 3 Controller. 

The PS3 Controller info is processed using the PS3USB Library.  The processed Variables/Values from the PS3USB Library are then wirelessly transmitted to the Quadcopter using 2 XBEE radios (one at the PS3 controller which is USB or Bluetooth connected to the transmitting Arduino and one at the Quadcopter connected to the receiving Arduino).

The other controls are as follows:
LeftStickY - Throttle
RightStickY - Pitch
RightStickX - Roll
L2/R2 - Yaw

Eventually, once I get the Auto-stabilization programmed and the Quadcopter flying, I plan to use the Arrow Buttons for 3-Axis GoPro Gimbal control.

Hardware:

Transmitter:
- Arduino Uno - AVR Microcontroller Processing 
- USB Host Shield (MAX3421E) - Receives Bluetooth Communication from PS3 Controller (SPI)
- XBEE Pro S1 with XBEE Shield - Transmits Serial Data to Quadcopter Receiver 
- PS3 Controller (Knockoff)

Quadcopter Receiver:
- Arduino Uno - AVR Microcontroller Processing (Flight/Motor Controller) 
- Invensense MPU-6050 6 Axis Gyro+Accelerometer (I2C)
- XBEE Pro S1 with XBEE Shield - Receives Serial Data from PS3 Controller Transmitter



__________________________________________________________________________________________________________

Quadcopter PS3 Control Test - Partial Pass - Runaway condition after erratic PS3 Controller inputs
https://youtu.be/OaTgQRiHxQk?si=IxaTVLx_bhIJYN5x

1st Test of Arduino XBEE Quadcopter PS3 Controller Inputs with Propellers On
https://youtu.be/Bv3kGazbDBQ?si=_ObbkUO-n4Kgytml

PS3 Controller, XBEE, Arduino Quadcopter - Communication Testing - Runaway Motors
https://youtu.be/gFub7hY7kV4?si=-8ukXQfpaxDtSD2T
