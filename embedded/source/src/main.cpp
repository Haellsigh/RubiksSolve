#include <Arduino.h>

#include <AccelStepper.h>
#include <MultiStepper.h>
#include <USBSerial.h>

#include <MotorConfiguration.h>
#include <commandmanager.h>

// Motor configurations
rubiks::MotorConfiguration motor[6] = {
#if defined(STM32F401)
    // step, dir
    {PB14, PB13},  // Moteur 1, blanc
    {PB15, PB1},   // Moteur 2, bleu
    {PB3, PA10},   // Moteur 3, rouge
    {PB5, PB4},    // Moteur 4, orange
    {PA8, PB10},   // Moteur 5, vert
    {PA9, PC7}     // Moteur 6, jaune
#elif defined(STM32F303)
    {PA12, PB0},  //
    {PB7, PB6},   //
    {PB1, PF0},   //
    {PF1, PA8},   //
    {PA11, PB5},  //
    {PA5, PA4}    //
#endif
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
    // stepper[i].setAcceleration(1);
    steppers.addStepper(stepper[i]);
  }
}

void loop() {
  // Parse bytes one by one
  while (Serial.available() > 0) {
    int data = Serial.read();
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

    delay(50);

    Serial.print("1: ");
    Serial.print(positions[0]);
    Serial.print(", 2: ");
    Serial.print(positions[1]);
    Serial.print(", 3: ");
    Serial.print(positions[2]);
    Serial.print(", 4: ");
    Serial.print(positions[3]);
    Serial.print(", 5: ");
    Serial.print(positions[4]);
    Serial.print(", 6: ");
    Serial.println(positions[5]);

    // Schedule the movement
    steppers.moveTo(positions);
  }
}