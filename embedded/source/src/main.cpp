#include <Arduino.h>

#include <AccelStepper.h>
#include <MultiStepper.h>

#include <commander.h>

struct MotorConfiguration {
  uint32_t stepsPerRevolution;
  uint32_t microstepping;

  uint32_t maxRPM;

  uint32_t getMaxStepsPerSecond() {
    return microstepping * stepsPerRevolution * maxRPM / 60.;
  }

  uint32_t getQuarter() { return (stepsPerRevolution * microstepping) * 0.25; }
  // uint32_t getHalf() { return (stepsPerRevolution * microstepping) * 0.5; }
};

struct MotorPins {
  uint8_t step;
  uint8_t dir;
};

// Motor configuration
MotorConfiguration config = {.stepsPerRevolution = 200,
                             .microstepping      = 8,
                             .maxRPM             = 200};

MotorPins pins[6] = {
    {PB13, PB14},  //
    {PB15, PB1},   //
    {PA4, PB0},    //
    {PC1, PC0},    //
    {PB10, PB4},   //
    {PB5, PB3}     //
};

rubiks::Commander commander;

AccelStepper motor1(AccelStepper::DRIVER, pins[0].step, pins[0].dir);
AccelStepper motor2(AccelStepper::DRIVER, pins[1].step, pins[1].dir);
AccelStepper motor3(AccelStepper::DRIVER, pins[2].step, pins[2].dir);
AccelStepper motor4(AccelStepper::DRIVER, pins[3].step, pins[3].dir);
AccelStepper motor5(AccelStepper::DRIVER, pins[4].step, pins[4].dir);
AccelStepper motor6(AccelStepper::DRIVER, pins[5].step, pins[5].dir);
MultiStepper steppers;

long positions[6] = {0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 6; i++) {
    pinMode(pins[i].step, OUTPUT);
    pinMode(pins[i].dir, OUTPUT);
  }

  uint32_t maxSpeed = config.getMaxStepsPerSecond();
  motor1.setMaxSpeed(maxSpeed);
  motor2.setMaxSpeed(maxSpeed);
  motor3.setMaxSpeed(maxSpeed);
  motor4.setMaxSpeed(maxSpeed);
  motor5.setMaxSpeed(maxSpeed);
  motor6.setMaxSpeed(maxSpeed);

  motor1.setAcceleration(10);
  motor2.setAcceleration(10);
  motor3.setAcceleration(10);
  motor4.setAcceleration(10);
  motor5.setAcceleration(10);
  motor6.setAcceleration(10);

  steppers.addStepper(motor1);
  steppers.addStepper(motor2);
  steppers.addStepper(motor3);
  steppers.addStepper(motor4);
  steppers.addStepper(motor5);
  steppers.addStepper(motor6);
}

void loop() {
  while (Serial.available() > 0) {
    int data = Serial.read();
    if (data >= 0) {
      commander.receiveNewData(data);
    }
  }

  // If all motors are stopped, run next command
  if (!steppers.run() && commander.hasNextCommand()) {
    auto cmd = commander.getNextCommand();

    size_t motor = cmd.motor + 1;
    positions[motor] += cmd.movement * config.getQuarter();
    steppers.moveTo(positions);
  }
}