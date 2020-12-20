// 1 bouton
// accelstepper => 1 moteur

#include <AccelStepper.h>
#include <MultiStepper.h>

AccelStepper stepper(AccelStepper::DRIVER, PB14, PB13);
MultiStepper steppers;

#include <Wire.h>
#include "AS5600.h"

AMS_5600 ams5600;

int ang, lang = 0;

float convertRawAngleToDegrees(word newAngle)
{
  /* Raw data reports 0 - 4095 segments, which is 0.087 of a degree */
  float retVal = newAngle * 0.087;
  ang = retVal;
  return ang;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  /*stepper.setMaxSpeed(8 * 200 * 250 / 60.);
  steppers.addStepper(stepper);*/
}

long position = 0;

void loop() {
  if(ams5600.detectMagnet())
  {
      Serial.println(String(convertRawAngleToDegrees(ams5600.getRawAngle()),DEC));
  }
  delay(50);
  /*
  while (Serial.available() > 0) {
    int data = Serial.read();
    if (data >= 0) {
      position += 200 * 8 / 4; // 90 deg
    }
  }

  if (!steppers.run()) {
    steppers.moveTo(&position);
  }
  */
}
