//--------------------------------------------------------------------------//
// Matt Corriveau  9/3/2017   Arduino Flight Controller Main
//
// This code uses Operator inputs from a PS3 Controller to control a Quadcopter
// The PS3 Controller info is processed using the PS3USB Library
// The processed Variables/Values from the PS3USB Library are then wirelessly transmitted to the Quadcopter using XBEE radios at the controller and at the quadcopter
// This code section is the flight controller processing.  It converts PS3 Controller Inputs (and eventually MPU-6050 Inputs) to Motor Outputs.
//
//--------------------------------------------------------------------------// 

 
 //------------------------------------------------------------// 
                  //  Write Collective Throttle Values  //
 //------------------------------------------------------------//

void collective()                                                                                                  // Up = 255
{
  PS3_Throttle = map(InLeftHatY, 0, 255, 1250, 1800);                    
    if (PS3_Throttle > 1800) PS3_Throttle = 1800;                                                                  // Need to keep room at full throttle for directional control (since motors can go up to 2000us pulses for full throttle)
                                                                                                                   // If we allow collective to control throttle up to 100%, there is no room for motors to increase speed for directional control at high collective throttle conditions.
}

 //------------------------------------------------------------// 
                  //  Write Pitch Throttle Values  //
 //------------------------------------------------------------//
 
//Verified Logic 9/4/2017                                                           // Motor Positions below:
void pitch()                                                                        //  0:LR    |   1:LF    |;   2:RF    |   3:RR                Aft = 255
{

  PS3_Pitch = map(InRightHatY, 0, 255, -127, 127);                                  //        Aft = 127      |      Fwd = -127
                                                                                    // This mapping was done for ease of algebraic logic in TotalThrottle values (note sign changes from TotalThrottle motor value to the next)
}

 //------------------------------------------------------------// 
                  //  Write Roll Throttle Values  //
 //------------------------------------------------------------//

//Verify when not tired
void roll()                                  //  0:LR    |   1:LF    |;   2:RF    |   3:RR                    Right = 255
{

  PS3_Roll = map(InRightHatX, 0, 255, -127, 127);                                   //        Right = 127      |      Left = -127
                                                                                    // This mapping was done for ease of algebraic logic in TotalThrottle values (note sign changes from TotalThrottle motor value to the next)
}

 //------------------------------------------------------------// 
                  //  Write Yaw Throttle Values  //
 //------------------------------------------------------------//
 
void yaw()                                //  0:LR    |   1:LF    |;   2:RF    |   3:RR                    Right = 255
{

  if(InL2 < 100){                                                                   // Want to be sure yaw values are set to zero before trigger gets into low resolution zone (not the best potentiometers in the knock off controller triggers)
  InL2 = 0;
 }
 
  if(InR2 < 100){
  InR2 = 0;                                                                         // Want to be sure yaw values are set to zero before trigger gets into low resolution zone (not the best potentiometers in the knock off controller triggers)
  InL2 = 0;
 }
 
 InL2 = map(InL2, 0, 255, 0, -255);                                                 // This mapping was done for ease of algebraic logic in TotalThrottle values (note sign changes from TotalThrottle motor value to the next)
 
 //Debug - to verify correct mapping
 //Serial.print("InL2 ");
 //Serial.println(InL2);
 
 PS3_Yaw = InL2 + InR2;                                                             //        Right = 127      |      Left = -127             Sum the two trigger values together.  Creates a single yaw variable and handles the "what if they are both pressed?" case.

}

/////////////////////////////////////////////////////////////////////////////////////////////////
                          // Sum Total Throttle and Set Motor Speeds  //                                  //Does not map motors all the way to zero, dont want motors turning off completely in rapid decents
/////////////////////////////////////////////////////////////////////////////////////////////////

void SumControlValuesforTotalThrottle(){                                            // Sums the Throttle, Pitch, Roll, and Yaw variables together for each of the 4 motors
  TotalThrottle0 = PS3_Throttle + PS3_Pitch - PS3_Roll + PS3_Yaw;                   // Note the algebraic logic in TotalThrottle values (ie. the sign changes (+ or -) from TotalThrottle motor value to the next)
  if (TotalThrottle0 > 2000) TotalThrottle0 = 2000;                                 // This logic is what controls whether a control input speeds a motor up, or slows a motor down, based on each specific control input.
  if (TotalThrottle0 < 1250) TotalThrottle0 = 1250;

  TotalThrottle1 = PS3_Throttle - PS3_Pitch - PS3_Roll - PS3_Yaw;                   // logic keeps the TotalThrottle variables between a 1000us pulse for motor full off, and 2000us pulse for motor full on
  if (TotalThrottle1 > 2000) TotalThrottle1 = 2000;                                 // since we do not want the motors full off in a rapid decent, we only slow them to 1250 (~10% throttle)
  if (TotalThrottle1 < 1250) TotalThrottle1 = 1250;

  TotalThrottle2 = PS3_Throttle - PS3_Pitch + PS3_Roll + PS3_Yaw;
  if (TotalThrottle2 > 2000) TotalThrottle2 = 2000; 
  if (TotalThrottle2 < 1250) TotalThrottle2 = 1250;

  TotalThrottle3 = PS3_Throttle + PS3_Pitch + PS3_Roll - PS3_Yaw;
  if (TotalThrottle3 > 2000) TotalThrottle3 = 2000; 
  if (TotalThrottle3 < 1250) TotalThrottle3 = 1250;
}


void SetMotorSpeed()                                                                // Set the motor speeds 
{
  if(EmergencyStopActive == 1){                                                     // If the Quad is in the EmergencyStopActive state, set motor speeds to zero (1000us pulse to each motor).
    esc0.writeMicroseconds(1000);
    esc1.writeMicroseconds(1000);
    esc2.writeMicroseconds(1000);
    esc3.writeMicroseconds(1000);
  }

  else{                                                                             // If the Quad is not in the EmergencyStopActive state, set motor speeds based on the TotalThrottle value of each motor.                                                                    
  //ESC 0
  val0 = TotalThrottle0;                                                            // Could eventually do away with this line and just call TotalThrottle0 into the writeMicroseconds function.  For now, write TotalThrottle0 into val0
  esc0.writeMicroseconds(val0);                                                     // Write val0 to the ESC (will always be between 1000us pulse for motor full off, and 2000us pulse for motor full on)
    
  //ESC 1
  val1 = TotalThrottle1;
  esc1.writeMicroseconds(val1);  
       
  //ESC 2
  val2 = TotalThrottle2;
  esc2.writeMicroseconds(val2);

  //ESC 3                                                        
  val3 = TotalThrottle3;
  esc3.writeMicroseconds(val3);

  
    if(i == 10000){                                                                 // Debug - counter results in a slower, more human readable, serial print for testing
    Serial.print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");                               // Print lots of new lines bewteen serial.prints to create a clear visual of the Motorspeeds
    
    Serial.print("3:\t");                                                           // Ordered 3, 0, 2, 1 for a Visual Depiction of the Quad in the Serial Print
    Serial.print(val3);                                                             // Print will look like...                                  
                                                                                    //                                                          
    Serial.print("\t\t\t0:\t");                                                     //                    3: 1650       0: 1650
    Serial.println(val0);                                                           //                    2: 1650       1: 1650
                                                                              
    Serial.print("2:\t");                                                           // Notice the printed values are in the physically correct positions per my quadcopter motor positions
    Serial.print(val2);                                                             // This makes testing, without actually running up the motors, very easy (...and quiet for parters who are also trying to work)
  
    Serial.print("\t\t\t1:\t");
    Serial.println(val1);
  
    i = 0;
    }                   // goes with serial print counter
    i++;
    
  }                     // goes with !EmergencyStopActive
}                       // goes with SetMotorSpeed

