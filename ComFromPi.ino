#include <Wire.h>
#include "MS5837.h"
#include <time.h>
#include <Servo.h>
#include <String.h>

  byte servo2_Pin = 2;                    // Attached Thruster 1 to AO-2   (Left)
  byte servo3_Pin = 3;                    // Attached Thruster 2 to AO-3   (Forward and Backward)
  byte servo4_Pin = 4;                    // Attached Thruster 3 to AO-4   (Forward and Backward)
  byte servo5_Pin = 5;                    // Attached Thruster 4 to AO-5   (Right)
  byte servo6_Pin = 6;                    // Attached Thruster 5 to AO-6   (Submerge)
  byte servo7_Pin = 7;                    // Attached Thruster 6 to AO-7   (Submerge)
  byte servoPin   = 8;

  
  Servo servo2;                           // 'Servo' is a special data structure type descriptor that exists in Python (make it easy to link to servo)
  Servo servo3;
  Servo servo4;
  Servo servo5;
  Servo servo6;
  Servo servo7;
  Servo servo8;

MS5837 sensor;
    
void setup(){

Serial.begin(9600);

servo2.attach(servo2_Pin);
servo3.attach(servo3_Pin);
servo4.attach(servo4_Pin);
servo5.attach(servo5_Pin);
servo6.attach(servo6_Pin);
servo7.attach(servo7_Pin);
servo8.attach(servoPin);

servo8.writeMicroseconds(1400);       // Initialize the light.
servo2.writeMicroseconds(1500);      // Initialize ALL Thrusters; send "STOP" signal to each ESC.
servo3.writeMicroseconds(1500);
servo4.writeMicroseconds(1500);
servo5.writeMicroseconds(1500);
servo6.writeMicroseconds(1500);
servo7.writeMicroseconds(1500);

delay(1000);                        // delay 1 second to allow each Thruster's ESC to recognize the "STOP" signal.


    
    //Forward  (1501 till 1900)
    //Stop     (1500)
    //Backward (1100 till 1499)

Wire.begin();
sensor.init();
sensor.setFluidDensity(997);


}

void thruster(int command){
while(command = 1){
  servo3.writeMicroseconds(1600);
  servo4.writeMicroseconds(1600);
    }
while(command = 2){
  servo2.writeMicroseconds(1500);
  servo3.writeMicroseconds(1500);
  servo4.writeMicroseconds(1500);
  servo5.writeMicroseconds(1500);
    }
while(command = 3){
  servo3.writeMicroseconds(1400);
  servo4.writeMicroseconds(1600);
    }
while(command = 4){
  servo3.writeMicroseconds(1600);
  servo4.writeMicroseconds(1400);
    }
while(command = 5){
  servo6.writeMicroseconds(1600);
  servo7.writeMicroseconds(1600);
    }
}
void loop(){
Serial.begin(9600);
if(Serial.available() > 0){
thruster(Serial.read());
    }
//int on = 1400;
//while(on != 0){
//servo8.writeMicroseconds(on); //Send signal to lumen light
//    }
sensor.read();
float pressure = sensor.pressure();
int timeleft = 4000;
while(millis() < timeleft){
if(pressure < 240){
      servo6.writeMicroseconds(1600);
      servo7.writeMicroseconds(1600);      
        }
while(millis() > timeleft)
if(pressure < 90){
      servo6.writeMicroseconds(1600);
      servo7.writeMicroseconds(1600);
    }
}
     
delay(500);

servo2.writeMicroseconds(1500);      // Send "STOP" signal to each ESC.
servo3.writeMicroseconds(1500);
servo4.writeMicroseconds(1500);
servo5.writeMicroseconds(1500);
servo6.writeMicroseconds(1500);
servo7.writeMicroseconds(1500);
    
    }
  







