#ifndef RUBIKSSOLVE_STEPPER_H
#define RUBIKSSOLVE_STEPPER_H

#include <cstdint>

namespace rubiks {

struct MotorConfiguration {
  MotorConfiguration(uint8_t step, uint8_t dir) : step(step), dir(dir) {}

  void init() {
    pinMode(step, OUTPUT);
    pinMode(dir, OUTPUT);
  }

  uint8_t step, dir;

  uint32_t stepsPerRevolution = 200;
  uint32_t microstepping      = 8;

  uint32_t maxRPM = 200;

  uint32_t maxStepsPerSecond() {
    if (mMaxStepsPerSecondComputed)
      return mMaxStepsPerSecond;
    else {
      mMaxStepsPerSecond         = microstepping * stepsPerRevolution * maxRPM / 60.;
      mMaxStepsPerSecondComputed = true;
      return mMaxStepsPerSecond;
    }
  }

  int32_t quarterTurn() {
    if (mQuarterComputed)
      return mQuarterTurn;
    else {
      mQuarterTurn     = (stepsPerRevolution * microstepping) / 4;
      mQuarterComputed = true;
      return mQuarterTurn;
    }
  }

 private:
  bool     mQuarterComputed           = false;
  bool     mMaxStepsPerSecondComputed = false;
  uint32_t mMaxStepsPerSecond;
  uint32_t mQuarterTurn;
};

}  // namespace rubiks

#endif