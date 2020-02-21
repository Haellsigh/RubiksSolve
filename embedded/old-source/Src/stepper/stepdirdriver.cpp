#include <stepper/stepdirdriver.h>

#include "stm32f4xx_hal.h"

namespace stepper {

void StepDirDriver::delay(uint32_t us)  // microseconds
{
  uint32_t startTick = DWT->CYCCNT;
  uint32_t delayTicks = us * (SystemCoreClock / 1e6);

  while (DWT->CYCCNT - startTick < delayTicks)
    ;
}

StepDirDriver::StepDirDriver(GPIO_TypeDef* dirPort,
                             uint16_t dirPin,
                             GPIO_TypeDef* stepPort,
                             uint16_t stepPin)
    : m_dirPort(dirPort),
      m_dirPin(dirPin),
      m_stepPort(stepPort),
      m_stepPin(stepPin) {
  // Set default direction to forward
  HAL_GPIO_WritePin(m_dirPort, m_dirPin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(stepPort, stepPin, GPIO_PIN_RESET);

  // Initializes us delay
  if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  }
}

void StepDirDriver::step(Direction dir, uint32_t n) {
  // Direction changed
  if (dir != m_currentDirection) {
    if (dir == Forward)
      HAL_GPIO_WritePin(m_dirPort, m_dirPin, GPIO_PIN_RESET);
    else
      HAL_GPIO_WritePin(m_dirPort, m_dirPin, GPIO_PIN_SET);
    m_currentDirection = dir;
  }

  for (uint32_t i = 0; i < n; i++) {
    HAL_GPIO_WritePin(m_stepPort, m_stepPin, GPIO_PIN_SET);
    delay(0);
    HAL_GPIO_WritePin(m_stepPort, m_stepPin, GPIO_PIN_RESET);
    delay(0);
  }
}

}  // namespace stepper
