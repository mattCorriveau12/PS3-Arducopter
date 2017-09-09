/*
 Example sketch for the PS3 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS3USB.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
/* You can create the instance of the class in two ways */
PS3USB PS3(&Usb); // This will just create the instance
//PS3USB PS3(&Usb,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

///////////////  define  ///////////////////
bool printAngle;
uint8_t state = 0;

  //int   In3 = 0;








void setup() {
  Serial.begin(9600);       /////////// changed from 115200 to interface with XBEE 9600 Baud Rate - Can try increasing this if quad control is lagging
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 USB Library Started"));
}

void loop() {
  Usb.Task();

  if (PS3.PS3Connected){
    
    //if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 )  
      if (PS3.getAnalogHat(LeftHatX) >= 0 )
      {
      Serial.print(F("\nLeftHatX: "));
      Serial.print(PS3.getAnalogHat(LeftHatX));
      Serial.print(F("\nLeftHatY: "));
      Serial.print(PS3.getAnalogHat(LeftHatY));   }
      
      
    //if (PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117)
    if (PS3.getAnalogHat(RightHatX) >= 0 )
    {
    Serial.print(F("\nRightHatX: "));
    Serial.print(PS3.getAnalogHat(RightHatX));
    Serial.print(F("\nRightHatY: "));
    Serial.print(PS3.getAnalogHat(RightHatY));  }
    
    
    // Analog button values can be read from almost all buttons
    if (PS3.getAnalogButton(L2)) {
      Serial.print(F("\nL2: "));
      Serial.print(PS3.getAnalogButton(L2));
      }
      
    if (PS3.getAnalogButton(R2)) {
      Serial.print(F("\nR2: "));
      Serial.print(PS3.getAnalogButton(R2));
      }
    
    if (PS3.getButtonClick(PS)){
      Serial.print(F("\nPS: "));
      Serial.print(PS3.getButtonClick(PS));}
      

    if (PS3.getButtonClick(TRIANGLE)){
      Serial.print(F("\nTraingle: "));
      Serial.print(PS3.getButtonClick(TRIANGLE));}
      
    if (PS3.getButtonClick(CIRCLE)){
      Serial.print(F("\nCircle: "));
      Serial.print(PS3.getButtonClick(CIRCLE));}
      
    if (PS3.getButtonClick(CROSS)){
      Serial.print(F("\nCross: "));
      Serial.print(PS3.getButtonClick(CROSS));}
      
    if (PS3.getButtonClick(SQUARE)){
      Serial.print(F("\nSquare: "));
      Serial.print(PS3.getButtonClick(SQUARE));}

    if (PS3.getButtonClick(UP)) {
      Serial.print(F("\nUpButton: "));
      Serial.print(PS3.getButtonClick(UP));
    }
    if (PS3.getButtonClick(RIGHT)) {
      Serial.print(F("\nRightButton: "));
      Serial.print(PS3.getButtonClick(RIGHT));
    }
    if (PS3.getButtonClick(DOWN)) {
      Serial.print(F("\nDownButton: "));
      Serial.print(PS3.getButtonClick(DOWN));
    }
    if (PS3.getButtonClick(LEFT)) {
      Serial.print(F("\nLeftButton: "));
      Serial.print(PS3.getButtonClick(LEFT));
    }

    if (PS3.getButtonClick(L1)){
      Serial.print(F("\nL1: "));
      Serial.print(PS3.getButtonClick(L1));}
      
    if (PS3.getButtonClick(L3)){
      Serial.print(F("\nL3: "));
      Serial.print(PS3.getButtonClick(L3));}
      
    if (PS3.getButtonClick(R1)){
      Serial.print(F("\nR1: "));
      Serial.print(PS3.getButtonClick(R1));}
      
    if (PS3.getButtonClick(R3)){
      Serial.print(F("\nR3: "));
      Serial.print(PS3.getButtonClick(R3));} 

    if (PS3.getButtonClick(SELECT)) {
      Serial.print(F("\nSelect: "));
      Serial.print(PS3.getButtonClick(SELECT));
    }
    
    if (PS3.getButtonClick(START)) {
      Serial.print(F("\nStart: "));
      Serial.print(PS3.getButtonClick(START));
    }
  

  }
}








