
#include <Wire.h>
#include "SFE_ISL29125.h"
#define defaultSpeed 120 
#define slowSpeed 80
#define greenRightSpeed 100
#define stopSpeed 0
#define turnTime 900
#define greenValueMin 1000
#define greenValueMax 2000
#define lightValueL 960  
#define darkValueL 1000
#define lightValue 925  
#define darkValue 1000


SFE_ISL29125 RGB_sensor;
int QRE1113_PinR = 0;
int QRE1113_PinM = 1;
int QRE1113_PinL = 2;
int rightMotor = 10;
int leftMotor = 11;

void setup() {
  //rgb
  Serial.begin(115200);
  pinMode(rightMotor, OUTPUT);
  pinMode(leftMotor, OUTPUT);
  if (RGB_sensor.init())
  {
    Serial.println("Sensor Initialization Successful\n\r");
  }  
}

void stopWheels(){
  analogWrite(rightMotor, stopSpeed);
  analogWrite(leftMotor, stopSpeed);
}

void shiftRight(){
  analogWrite(rightMotor, defaultSpeed);
  analogWrite(leftMotor, slowSpeed);
}

void shiftLeft(){
  analogWrite(leftMotor, defaultSpeed);
  analogWrite(rightMotor, slowSpeed);
}

void greenTurnRight(){
  analogWrite(leftMotor, 0);
  analogWrite(rightMotor, 0);
  
  analogWrite(leftMotor, 0);
  analogWrite(rightMotor, greenRightSpeed);
  delay(turnTime);
}


void loop() {

  //check green
  unsigned int green = RGB_sensor.readGreen();
  if (green < greenValueMax && green > greenValueMin){
    Serial.print("Green: "); Serial.println(green);
    Serial.print("green detected");
    greenTurnRight();
  }

   


  //Check line sensors to stay on line
  while((analogRead(QRE1113_PinR) > darkValue || analogRead(QRE1113_PinR) < lightValue)&&(analogRead(QRE1113_PinL) > darkValue || analogRead(QRE1113_PinL) < lightValue)&&
  (analogRead(QRE1113_PinM) > darkValue || analogRead(QRE1113_PinM) < lightValue)&&(green > greenValueMax || green < greenValueMin)){
    Serial.print("Stop wheels");
     Serial.print("R: "); Serial.println(analogRead(QRE1113_PinR));
     Serial.print("M: "); Serial.println(analogRead(QRE1113_PinM));
     Serial.print("L: "); Serial.println(analogRead(QRE1113_PinL));
    Serial.println();
    stopWheels();
          
  }
  
  
  
  while(((analogRead(QRE1113_PinR) > darkValue || analogRead(QRE1113_PinR) < lightValue)&&
  (analogRead(QRE1113_PinM) > darkValue || analogRead(QRE1113_PinM) < lightValue))&&(analogRead(QRE1113_PinL)>lightValueL && analogRead(QRE1113_PinL)<darkValueL)){
    Serial.print("Shift left"); 
    Serial.print("R: "); Serial.println(analogRead(QRE1113_PinR));
     Serial.print("M: "); Serial.println(analogRead(QRE1113_PinM));
     Serial.print("L: "); Serial.println(analogRead(QRE1113_PinL));
    Serial.println();
    shiftLeft();
  }

  while((analogRead(QRE1113_PinL) > darkValueL || analogRead(QRE1113_PinL) < lightValueL)&&
  (analogRead(QRE1113_PinM) > darkValue || analogRead(QRE1113_PinM) < lightValue)&&(analogRead(QRE1113_PinR)>lightValue && analogRead(QRE1113_PinR)<darkValue)){
    Serial.print("Shift right");
    Serial.println();
    shiftRight();
  }

  //reset wheels
  analogWrite(rightMotor, defaultSpeed);
  analogWrite(leftMotor, defaultSpeed); 

  
  delay(10);


}
