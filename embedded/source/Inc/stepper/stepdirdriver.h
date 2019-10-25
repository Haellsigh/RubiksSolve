#ifndef RUBIKSSOLVE_STEPPER_STEPDIRDRIVER_H_
#define RUBIKSSOLVE_STEPPER_STEPDIRDRIVER_H_

#include <stepper/driver.h>
#include "stm32f4xx_hal.h"

namespace stepper {

class StepDirDriver : public Driver {
 public:
  StepDirDriver(GPIO_TypeDef* dirPort,
                uint16_t dirPin,
                GPIO_TypeDef* stepPort,
                uint16_t stepPin);

  void step(Direction dir = Forward, uint32_t n = 1) override;

  void delay(uint32_t us);

 private:
  Direction m_currentDirection = Forward;

  GPIO_TypeDef* m_dirPort;
  uint16_t m_dirPin;
  GPIO_TypeDef* m_stepPort;
  uint16_t m_stepPin;
};

}  // namespace stepper

#endif  // RUBIKSSOLVE_STEPPER_STEPDIRDRIVER_H_
