///////////////////////////////////////////////////////////////////////////////////////
//Terms of use
///////////////////////////////////////////////////////////////////////////////////////
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////////////
//Safety note
///////////////////////////////////////////////////////////////////////////////////////
//Always remove the propellers and stay away from the motors unless you 
//are 100% certain of what you are doing.
///////////////////////////////////////////////////////////////////////////////////////


//--------------------------------------------------------------------------//
// Matt Corriveau  9/3/2017   Arduino Flight Controller Main
//
// This code uses Operator inputs from a PS3 Controller to control a Quadcopter
// The PS3 Controller info is processed using the PS3USB Library
// The processed Variables/Values from the PS3USB Library are then wirelessly transmitted to the Quadcopter using XBEE radios at the controller and at the quadcopter
// This code section is the ESC Start Up Sequence.  All Motors --> Min Throttle 5 secs, Max Throttle 1 sec, Min Throttle 1 sec.
// This start sequence was determined through trial and error since my Amazon ESCs didnt come with any user guide or part number.
//
//--------------------------------------------------------------------------// 

// Start Up Sequene Notes                                 

// Note that the first time the PS3 Controller is powered on, the PS3 Button needs to pressed before any of the controls will write to the quadcopter.  Haven't looked
// for this in the PS3USB code yet, but I have empirically determined this to be the case.  I have taken care of this for you by placing the Quad into the 
// EmergencyStopActive state immediately upon completing the start-up sequence.

// The throttle should be held in the full down position before pressing the PS3 Button to take the Quad out of the EmergencyStopActive state.  I have not programmed 
// this in as a fail safe yet.  The motors will respond to the throttle position immediately upon pressing the PS3 Button, after the start sequence has been completed.

//--------------------------------------------------------------------------// 
                                                          
void StartUpSequenceESC(){                                // ESCs (Electronic Speed Controllers) require a start up sequence as a safety precaution

Serial.print("\n\n\n\n\n");                               // Serial.print for testing
Serial.print("ESC Arming...");
Serial.print("\n");
delay(50);

/////////////////////////////////////////////////////////////////////////////////////////////////
                                 // Initialize Min  //
/////////////////////////////////////////////////////////////////////////////////////////////////

  TotalThrottle0 = 0;                                     // Start by setting all Motors to off
  TotalThrottle1 = 0;
  TotalThrottle2 = 0;
  TotalThrottle3 = 0;
  
  SetInitialMotorSpeed();                                 // Call SetInitialMotorSpeed.  We dont use SetMotorSpeed, because we set motor speed doesn't allow speeds below 1250us for the rapid decents (this was coded this way before the EmergencyMotorStop function).

                                                          // Print the speeds to the serial monitor:
  Serial.print("\n");
  Serial.print("In0 = ");
  Serial.print(TotalThrottle0);
  Serial.print("\t Out0 = ");
  Serial.println(val0);

  Serial.print("In1 = ");
  Serial.print(TotalThrottle1);
  Serial.print("\t Out1 = ");
  Serial.println(val1);

  Serial.print("In2 = ");
  Serial.print(TotalThrottle2);
  Serial.print("\t Out2 = ");
  Serial.println(val2);
  
  Serial.print("In3 = ");
  Serial.print(TotalThrottle3);
  Serial.print("\t Out3 = ");
  Serial.println(val3);
  
delay(5000);                                                //Delay 5 seconds (not sure it needs to be quiet this long)

/////////////////////////////////////////////////////////////////////////////////////////////////
                                 // Initialize Max  //
/////////////////////////////////////////////////////////////////////////////////////////////////

  TotalThrottle0 = 255;                                     // Set all Motors to 100% throttle
  TotalThrottle1 = 255;
  TotalThrottle2 = 255;
  TotalThrottle3 = 255;
  
  SetInitialMotorSpeed();                                   // Call SetInitialMotorSpeed. 
  
                                                            // Print the speeds to the serial monitor:
  Serial.print("\n");
  Serial.print("In0 = ");
  Serial.print(TotalThrottle0);
  Serial.print("\t Out0 = ");
  Serial.println(val0);

  Serial.print("In1 = ");
  Serial.print(TotalThrottle1);
  Serial.print("\t Out1 = ");
  Serial.println(val1);  

  Serial.print("In2 = ");
  Serial.print(TotalThrottle2);
  Serial.print("\t Out2 = ");
  Serial.println(val2);
  
  Serial.print("In3 = ");
  Serial.print(TotalThrottle3);
  Serial.print("\t Out3 = ");
  Serial.println(val3);

  
delay(500);                                                 //Delay 500ms so motors see this value

/////////////////////////////////////////////////////////////////////////////////////////////////
                                 // Initialize Min  //
/////////////////////////////////////////////////////////////////////////////////////////////////

  TotalThrottle0 = 0;                                     // Set all Motors to 0% throttle so quadcopter does lift off immediately upon start up
  TotalThrottle1 = 0;
  TotalThrottle2 = 0;
  TotalThrottle3 = 0;
  
  SetInitialMotorSpeed();                                 // Call SetInitialMotorSpeed. 

                                                          // Print the speeds to the serial monitor:
  Serial.print("\n");
  Serial.print("In0 = ");
  Serial.print(TotalThrottle0);
  Serial.print("\t Out0 = ");
  Serial.println(val0);

  Serial.print("In1 = ");
  Serial.print(TotalThrottle1);
  Serial.print("\t Out1 = ");
  Serial.println(val1);

  Serial.print("In2 = ");
  Serial.print(TotalThrottle2);
  Serial.print("\t Out2 = ");
  Serial.println(val2);
  
  Serial.print("In3 = ");
  Serial.print(TotalThrottle3);
  Serial.print("\t Out3 = ");
  Serial.println(val3);

  
delay(500);                                                 //Delay 500ms so motors see this value     

/////////////////////////////////////////////////////////////////////////////////////////////////
            // Test Code - Run Motors at 15% throttle for 5 seconds after Startup  //
/////////////////////////////////////////////////////////////////////////////////////////////////

                                                            // End the Sequence with a "Crowd Pleasing" Taunt
  TotalThrottle0 = 100;                                     // Rev Motor 0 to 50% for 1/4 second (used 300ms for the first motor because it seems to lag a bit)                                
  SetInitialMotorSpeed();
  delay(300);  

  TotalThrottle0 = 0;                                       // Turn off Motor 0
  TotalThrottle1 = 100;                                     // Rev Motor 1 to 50% for 1/4 second  
  SetInitialMotorSpeed(); 
  delay(250);  


  TotalThrottle1 = 0;                                       // Turn off Motor 1
  TotalThrottle2 = 100;                                     // Rev Motor 2 to 50% for 1/4 second  
  SetInitialMotorSpeed();
  delay(250);  


  TotalThrottle2 = 0;                                       // Turn off Motor 2
  TotalThrottle3 = 100;                                     // Rev Motor 3 to 50% for 1/4 second  
  SetInitialMotorSpeed();
  delay(250);

  TotalThrottle3 = 0;                                       // Turn off Motor 3
  SetInitialMotorSpeed();
  delay(250);

                                                            // Print the speeds to the serial monitor. (Should all be 0 again at this point.)
  Serial.print("\n");
  Serial.print("In0 = ");
  Serial.print(TotalThrottle0);
  Serial.print("\t Out0 = ");
  Serial.println(val0);

  Serial.print("In1 = ");
  Serial.print(TotalThrottle1);
  Serial.print("\t Out1 = ");
  Serial.println(val1);

  Serial.print("In2 = ");
  Serial.print(TotalThrottle2);
  Serial.print("\t Out2 = ");
  Serial.println(val2);
  
  Serial.print("In3 = ");
  Serial.print(TotalThrottle3);
  Serial.print("\t Out3 = ");
  Serial.println(val3);

EmergencyStopActive = 1;                                    // Puts the Motors into the Emergency Start State immediately upon completing the start-up sequence.  To start, press the PS3 Button on the Controller.
                                                            // Note that the Motors will immediately output based on the throttle stick being in center position, so the throttle should be held low before pressing the
                                                            // PS3 button to take the Quad out of the EmergencyStopActive state.

}                                                           //Closes StartupSequenceESC

/////////////////////////////////////////////////////////////////////////////////////////////////
               // Set Motors Speed Inialization - Maps all the way to 0  //
/////////////////////////////////////////////////////////////////////////////////////////////////
 
void SetInitialMotorSpeed()                                           // Need this function since the SetMotorSpeed function will not go all the way down to zero (don't want motors to turn all the way off in rapid decents).
{
  //ESC 1
  val0 = map(TotalThrottle0, 0, 255, 1000, 2000);                     // Map input of 0 - 255 to output of 1000us (Motor off) to 2000us (motor 100% throttle).
  esc0.writeMicroseconds(val0);                                       // Write Val0 to the ESC.

  //ESC 2
  val1 = map(TotalThrottle1, 0, 255, 1000, 2000);
  esc1.writeMicroseconds(val1);
  
  //ESC 3
  val2 = map(TotalThrottle2, 0, 255, 1000, 2000);
  esc2.writeMicroseconds(val2);
  
  //ESC 4
  val3 = map(TotalThrottle3, 0, 255, 1000, 2000);
  esc3.writeMicroseconds(val3);
  
}




