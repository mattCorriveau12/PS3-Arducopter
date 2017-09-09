//--------------------------------------------------------------------------//
// Matt Corriveau  9/3/2017   Serial Parsing of XBEE Communication in AT Mode
//
// This code uses Operator inputs from a PS3 Controller to control a Quadcopter
// The PS3 Controller info is processed using the PS3USB Library
// The processed Variables/Values from the PS3USB Library are then wirelessly transmitted to the Quadcopter using XBEE radios at the controller and at the quadcopter
// This code section is how the Arduino on the Quadcopter parses the serial information send from the XBEE on the Ground (PS3 Controller data) and received by the XBEE on the Quadcopter
//
//--------------------------------------------------------------------------//
 
 //------------------------------------------------------------// 
                        //  Parse Serial Data  //
 //------------------------------------------------------------//

void ReadSerialData(){
  
if (Serial.available() >0 ) {                                                   // If there is serial information available in the buffer

    char c = Serial.read();                                                     // read that information into the variable "c"
    //Serial.print(c);                                                          // Debug - prints all serial data received so user can tell if the issue is a transmission issue or a parsing issue
      if(c == '\n'){                                                            // if there is a new line, assume a new command is being sent.  We'll have to parse out each data field if there are multiple pieces of data in each command (for our purposes, there will be)
        parseCommand(command);                                                  // breaks out of ReadSerialData based on where parse directs it
        command = "";                                                           // clears the data in this position of the "command" string
       }                                 
      else{                             
        command += c;                                                           // adds the data in "c" to the end of the "command" string
      }                                 
    }
   // Serial.println("\nListening for Serial Data...");                         // Debug - if Serial.print(c) shows values are received but questioning whether the issue is a parsing issue
}                                                                               // As a word of caution, because this hung me up for a while, ensure that the transmit program sends each individual control command on a new line,
                                                                                // using the /n command.  At first, my transmitter used the /r command to create a new line for every control command.  Since the parser did not every see
                                                                                // a /n (because it was seeing /r's), it never parsed the data.  Since we are using an if(c == '\n'){}, make sure you are sending a \n before every command.

void parseCommand(String com)                                                   // this will allow us to parse the different data pieces from each new command
{
  String part1;                                                                 // Variable Name of Specific Data Piece from "command"
  String part2;                                                                 // Value of Specific Data Piece from "command"
                                                                                // This parsing convention will assume the data will look like:
  part1 = com.substring(0, com.indexOf(" "));                                   //  "LHatX: 128   or  LHatY: 256  or   L2: 0  or   R2: 128"  //  Be sure Every command starts on a new line USING THE '\n' COMMAND!
  part2 = com.substring(com.indexOf(" ") + 1);                                  // The space is what defines the end of part1 and the beginning of part2
                                                               
                                          
                                                               
 //------------------------------------------------------------// 
          //  Left Hat Y - Collective / DeadZone//
//------------------------------------------------------------//

    if(part1.equalsIgnoreCase("LeftHatY:")){                                    // if new line starts with LeftHatY
      InLeftHatY = part2.toInt();                                               // convert the value after the text "LeftHatY" into an integer and pass it into the variable InLeftHatY
      InLeftHatY = constrain(InLeftHatY, 0, 255);                               // Stick Down = 255,    Stick Up = 0            Constrain so that we never have values outside of 0 and 255
/*                                                                              
 *                                                                              // I commented out the Dead Zone because my variables would not return to center after the stick was centered.  They would rememvber the last variable before the re-enterred the dead zone.
      // Left Hat Y Center Dead Zone                                            // Another solution would have been to use hysteresis
       if (InLeftHatY > 117 && InLeftHatY < 137) {                              // Sets dead zone in center of hat to prevent erroneous values - the dead zone is large to compensate for poor PS3 Controller Joystick Potentiometer Resolution in the low range (Knock off Dualshock 3 Controller)
        InLeftHatY = 0;                                                         // This probably shouldnt be zero when centered, but I haven't used this code in a while - sounds like a good research topic for the next guy/gal...
       }
       
      // RightHatY Forward                                                      // This was how I initially was processing the PS3 controller inputs, but I decieded there was no need for a forward and an aft function, since the stick cannot be in 2 positions at once
      else if (InLeftHatY < 137){                                               //LeftHatY
                //changed from Forward for debug                             

      }

      //RightHat Y Aft
      else if(InLeftHatY > 117){                                                //LeftHatY
                                    
        //Debug - Can remove the following lines if code is working
         
      }
        
        Serial.print("LeftHatY: ");
        Serial.println(InLeftHatY);
        */
        InLeftHatY = (InLeftHatY*(-1))+255;                                    //Conversion so Up = 255                converting so 255 was up made for easier correlation of Joystick Up to Motor Speed Up               
        
          /*                                                                   //Debug - counter results in a slower, more human readable, serial print for testing
          if (c==1000){
          Serial.print("LeftHatY: ");
          Serial.println(InLeftHatY);
            c = 0;
          }   goes with serial print counter
          c++;*/
          
        collective();                                                          // call collective function;
                                                              
  }  
 

//------------------------------------------------------------// 
      //  Right Hat Y - Pitch Forward / Aft / DeadZone//
//------------------------------------------------------------//

    if(part1.equalsIgnoreCase("RightHatY:")){                                  // if new line starts with RightHatY
      InRightHatY = part2.toInt();                                             // same idea as above
      InRightHatY = constrain(InRightHatY, 0, 255);                            // Constrain so that we never have values outside of 0 and 255
/*
      // Right Hat Y Center Dead Zone
       if (InRightHatY > 117 && InRightHatY < 137) {                           // Sets dead zone in center of hat to prevent erroneous values - the dead zone is large to compensate for poor PS3 Controller Joystick Potentiometer Resolution in the low range (Knock off Dualshock 3 Controller)
        InRightHatY = 128;                                                                  
       }
       
      // RightHatY Forward                                                     // This was how I initially was processing the PS3 controller inputs, but I decieded there was no need for a forward and an aft function, since the stick cannot be in 2 positions at once
      else if (InRightHatY < 137){                                             //Right Hat Forward
        PitchForward();        //changed from Forward for debug                           
      //Debug - Can remove the following lines if code is working
        Serial.print("RightHatY: ");
        Serial.println(InRightHatY);        
      }

      //RightHat Y Aft
      else if(InRightHatY > 117){                                             //Right Hat Aft
        PitchAft();         // changed from aft for debug                             
        //Debug - Can remove the following lines if code is working
        Serial.print("RightHatY: ");
        Serial.println(InRightHatY);         
      }
   */
          /*                                                                  //Debug - counter results in a slower, more human readable, serial print for testing
          if (p==1000){
          Serial.print("RightHatY: ");
          Serial.println(InRightHatY); 
            p = 0;
          }   //goes with serial print counter
          p++;*/
        
      pitch();                                                                 // call pitch function
                                                              
  }  

//------------------------------------------------------------// 
      //  Right Hat X  - Roll Left / Right / Dead Zone//
//------------------------------------------------------------//

    if(part1.equalsIgnoreCase("RightHatX:")){                                  // if new line starts with RightHatX
      InRightHatX = part2.toInt();                                             // same idea as above
      InRightHatX = constrain(InRightHatX, 0, 255);                            // Constrain so that we never have values outside of 0 and 255

      /*
      // Right Hat X Center Dead Zone
       if (InRightHatX > 117 && InRightHatX < 137) {                           // Sets dead zone in center of hat to prevent erroneous values - the dead zone is large to compensate for poor PS3 Controller Joystick Potentiometer Resolution in the low range (Knock off Dualshock 3 Controller)
        InRightHatX = 128;                                 
       }
       
      // RightHatY Left                                                        // This was how I initially was processing the PS3 controller inputs, but I decieded there was no need for a forward and an aft function, since the stick cannot be in 2 positions at once
      else if (InRightHatX < 117){                                             //Right Hat Left
       RollLeft ();                                
      //Debug - Can remove the following lines if code is working
        Serial.print("RightHatX: ");
        Serial.println(InRightHatX);        
      }

      //RightHat X Right
      else if(InRightHatX > 137){                                             //Right Hat Right
        RollRight();                                   
        //Debug - Can remove the following lines if code is working
        Serial.print("RightHatX: ");
        Serial.println(InRightHatX);         
      }
      */

         /*
         if (r==1000){                                                        //Debug - counter results in a slower, more human readable, serial print for testing
            Serial.print("RightHatX: ");
            Serial.println(InRightHatX);
            r = 0;
          }   //goes with serial print counter
          r++;*/
          
      roll();                                                                 // call roll function
                                                              
  }  



 //------------------------------------------------------------// 
                    //  L2 Input - Yaw Left //
 //------------------------------------------------------------//

    if(part1.equalsIgnoreCase("L2:")){                                        //Read part1 (Not case sensitive) and look for the string "L2: "
      InL2 = part2.toInt();                                                   //Converts whatever is in part2 from a string to an integer value                        
      InL2 = constrain(InL2, 0, 255);                                         //"Collective" maps left trigger from 0 to 255 (will later mapped from 0 to -255 for Logic of the Total Motors Speeds Sums)
      
      /*
      //Debug - Can remove the following lines if code is working
      Serial.print("L2: ");
      Serial.println(InL2);  
                                                        
      if (InL2 < 75) {                                                        //Compensating for poor PS3 Controller Joystick Potentiometer Resolution in the low range (Knock off Dualshock 3 Controller - because I'm a bargain shopper...)
       InL2 = 0;                                                              //And paying the price in code instead
       }
      YawLeft();                                      
      */
 
        /*
        if (yL==1000){                                                        //Debug - counter results in a slower, more human readable, serial print for testing
          Serial.print("L2: ");
          Serial.println(InL2);
          yL = 0;
        }   //goes with serial print counter
        yL++;*/
           
      yaw();                                                                 // call yaw function (note both R2 and L2 triggers call the same function)
    }
    
//------------------------------------------------------------// 
                  //  R2 Input - Yaw Right  //
//------------------------------------------------------------//
    if(part1.equalsIgnoreCase("R2:")){
      InR2 = part2.toInt(); 
      InR2 = constrain(InR2, 0, 255); 
   /*     
      //Debug - Can remove the following lines if code is working
      Serial.print("R2: ");
      Serial.println(InR2);   
                                                       
       if (InR2 < 75) {                                                     //Again, compensating for poor PS3 Controller Joystick Potentiometer Resolution in the low range (Knock off Dualshock 3 Controller)
        InR2 = 0;                                                             
       }
      YawRight();                                                           
       */
        /*
        if (yR==1000){                                                      //Debug - counter results in a slower, more human readable, serial print for testing
          Serial.print("R2: ");
          Serial.println(InR2); 
          yR = 0;
        }   //goes with serial print counter
        yR++;*/
        
      yaw();                                                                // call yaw function (note both R2 and L2 triggers call the same function)
    }    


//------------------------------------------------------------// 
              //  Wait for PS to Restart Motors  //
//------------------------------------------------------------//
  if(part1.equalsIgnoreCase("PS:")){                                        // If the PS Button is pressed...
    InPS = part2.toInt();                                                   // I don't think this is doing anything since we never use the value in the InPS variable
                                                                            // I dont use the InPS variable because the PSUSB defines the InPS variable and does not have it latching the value of 1 when the button is pressed. The value stays 1 for just 1 cycle then returns to 0.
      if(EmergencyStopActive == 0){                                         // ... and if the Quadcopter is not in the EmergencyStopActive state...
        EmergencyStopActive = 1;                                            // Set the EmergencyStopActive value to 1, putting the Quad in the EmergencyStopActive state
        Serial.println("\tMotor Emergency Stop - ENABLED");                 // Alert the pilot that the Emergency Stop is Enabled
        SetMotorSpeed();                                                    // Run set motor speed.  (The SetMotorSpeed function starts by asking if the Quadcopter is iin the EmergencyStopActive.  If it is, it sets the motor speeds to off (1000ms pulse).)          
        }

      else{                                                                 // Still using the same PS Button Pressed from above, if the button is pressed ... and if the Quadcopter is in the EmergencyStopActive state...   
        EmergencyStopActive = 0;                                            // Set the EmergencyStopActive value to 0, taking the Quad out of the EmergencyStopActive state                                     
        Serial.println("\tMotor Emergency Stop - DISABLED");                // Alert the pilot that the Emergency Stop is Disabled
        SetMotorSpeed();                                                    // Run set motor speed.  (The SetMotorSpeed function starts by asking if the Quadcopter is iin the EmergencyStopActive.  If it is not, the motor speeds are set based on the TotalThrottle variables.          
      }
  }
    
   

//------------------------------------------------------------// 
                 //  Closing Brackets  //
//------------------------------------------------------------//

      
}                                                                             //Close Parse Serial Data (all PS3 commands stay within this bracket







