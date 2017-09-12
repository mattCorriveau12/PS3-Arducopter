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
// This code section is the Main file and includes Definitions, Setup Code, and the Main Loop which references multiple functions for readability
//
// The code is original code but makes use of some existing libraries (Servo and PS3USB). The Quadcopter Hardware is setup in accorance with the following video series:
// https://youtu.be/XFxqFQwRumc
//
// Libraries
// https://github.com/felis/USB_Host_Shield_2.0/blob/master/PS3USB.h
// https://www.youtube.com/watch?v=X5u2qCzcPn8
//
// Many thanks to the creators of the content at the links above for their helpful and informative material.
//--------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////////////////////////
                                 // Included Libraries //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <PS3USB.h>
#include <Servo.h>

/////////////////////////////////////////////////////////////////////////////////////////////////
                                 // Defined Variables  //
/////////////////////////////////////////////////////////////////////////////////////////////////

USB Usb;                                          // For PS3 Communication                                         
PS3USB PS3(&Usb);                                 // For PS3 Communication                  

Servo esc0;                                       // Define 4 motors
Servo esc1;
Servo esc2;
Servo esc3;

int val0;                                         // Motor write values
int val1;
int val2;
int val3;

int  TotalThrottle0;                              // Processed Motor speed Total values
int  TotalThrottle1; 
int  TotalThrottle2;
int  TotalThrottle3;

int PS3_Throttle;                                 // Processed Motor speed Throttle values (collective)

int PS3_Pitch;                                    // Processed Motor speed Pitch values

int PS3_Roll;                                     // Processed Motor speed Roll values

int PS3_Yaw;                                      // Processed Motor speed Yaw values

int EmergencyStopActive;                          //sets motor state to Emergency Stop
int i;                                            //serial print motor value counter variable (slows down serial print for human readability)
int c;                                            //serial print collective input value counter variable
int p;                                            //serial print pitch input value counter variable
int r;                                            //serial print roll input value counter variable
int yL;                                           //serial print yaw Left input value counter variable
int yR;                                           //serial print yaw Right input value counter variable
 
/////////////////////////////////////////////////////////////////////////////////////////////////
                                 // Defined PS3 Controller Variables  //
/////////////////////////////////////////////////////////////////////////////////////////////////

String command;                                   //for Serial Parsing

int InL2;                                         //PS3 controller Yaw input 
int InR2;                                         //PS3 controller Yaw input 
int InRightHatY;                                  //PS3 controller Pitch input
int InRightHatX;                                  //PS3 controller Roll input
int InLeftHatY;                                   //PS3 controller throttle input

int InPS;                                         //PS3 controller Playstation Button used for Emergency Motor Stop


/////////////////////////////////////////////////////////////////////////////////////////////////
                                          // Setup  //
/////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {                                    //Assign Arduino Pins to Motors
esc0.attach(4);
esc1.attach(5);
esc2.attach(6);
esc3.attach(7);

                                                  // initialize serial communications at 9600 bps:              //This will likely have to change, with the baud of the XBEE, to reduce control lag (10us update frequency)
  Serial.begin(9600);                             //for Serial Parsing, and Servo.h
  Serial.flush();                                 //not entirely sure this is necessary, we're parsing anyways
  delay(1);                                       //not entirely sure this is necessary

StartUpSequenceESC();                             //Perform ESC Start up Sequence - All Motors --> Min Throttle 5 secs, Max Throttle 250ms, Min Throttle 500ms, Rev each motor up for 250ms (the revving is for show)
                                                  //Determined this start cycle through trial and errors since my Amazon ESCs didnt come with any user guide or part number

}

/////////////////////////////////////////////////////////////////////////////////////////////////
                                          // Loop  //
/////////////////////////////////////////////////////////////////////////////////////////////////
 
void loop() {

//Read MPU-6050                                   //Placeholder - Read MPU-6050 Accelerometer and Gyro Information
//Auto-Stabilize                                  //Placeholder - Set motor speeds to keep gyro level
//Interrupt                                       //Placeholder - Change SetMotorSpeed to be Interupt Driven to allow for PS3 Control Input but keeping the MPU-6050 maintaining stabliilty between PS3 commands
                              
ReadSerialData();                                 //Read Serial Data from XBEE
SumControlValuesforTotalThrottle();               //Need each motors throttle speed to be a sum of collective, yaw, and cyclic
SetMotorSpeed();                                  //Set motor speeds in accordance with the PS3 Controller Input
                                 
}




 





 
  
  
  
  
  
  
 
