#include <Arduino.h>

#include <AccelStepper.h>
#include <MultiStepper.h>
#include <USBSerial.h>

#include <MotorConfiguration.h>
#include <commandmanager.h>

// Motor configurations
rubiks::MotorConfiguration motor[6] = {
    {PB13, PB14},  //
    {PB15, PB1},   //
    {PA4, PB0},    //
    {PC1, PC0},    //
    {PB10, PB4},   //
    {PB5, PB3}     //
};

rubiks::commandmanager commandManager;

AccelStepper stepper[6] = {
    {AccelStepper::DRIVER, motor[0].step, motor[0].dir},
    {AccelStepper::DRIVER, motor[1].step, motor[1].dir},
    {AccelStepper::DRIVER, motor[2].step, motor[2].dir},
    {AccelStepper::DRIVER, motor[3].step, motor[3].dir},
    {AccelStepper::DRIVER, motor[4].step, motor[4].dir},
    {AccelStepper::DRIVER, motor[5].step, motor[5].dir},
};
MultiStepper steppers;

long positions[6] = {0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 6; i++) {
    motor[i].init();
  }

  for (int i = 0; i < 6; i++) {
    stepper[i].setMaxSpeed(motor[0].maxStepsPerSecond());
    stepper[i].setAcceleration(10);
    steppers.addStepper(stepper[i]);
  }
}

void loop() {
  // Parse bytes one by one
  if (Serial.available() > 0) {
    int data = Serial.read();
    Serial.write(data);
    if (data >= 0) {
      commandManager.receiveNewData(data);
    }
  }

  // If all motors are stopped and we have a next command ready
  if (!steppers.run() && commandManager.hasNextCommand()) {
    // Retrieve next movement
    auto cmd = commandManager.getNextCommand();
    // Increment position
    size_t iMotor = cmd.motor - 1;
    positions[iMotor] += cmd.movement * motor[iMotor].quarterTurn();
    // Schedule the movement
    steppers.moveTo(positions);
  }
}