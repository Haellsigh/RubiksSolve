#include <cmath>

#include <stepper/driver.h>
#include <stepper/motor.h>

#include "stm32f4xx_hal.h"

namespace stepper {

int sgn(float val) {
  return (0.f < val) - (val < 0.f);
}

float round(float d) {
  return floor(d + 0.5f);
}

Motor::Motor(const float Vmax, const float Amax, const float Jmax) {
  setCharacteristics(Vmax, Amax, Jmax);
  m_ticksToSecond = 1. / SystemCoreClock;
  m_lastTick = DWT->CYCCNT;
}

void Motor::setDriver(Driver* driver) {
  m_driver = driver;
}

void Motor::setCharacteristics(const float Vmax,
                               const float Amax,
                               const float Jmax) {
  m_Vmax = Vmax;
  m_Amax = Amax;
  m_Jmax = Jmax;

  m_alpha = getMaxAlpha();
  m_Ts = 1.25 * 1.9045f / m_alpha;
}

void Motor::setStepPerUnit(const float factor) {
  m_stepPerUnit = factor;
  m_unitPerStep = 1.f / factor;
}

void Motor::update() {
  uint32_t dt = DWT->CYCCNT - m_lastTick;
  m_lastTick = DWT->CYCCNT;

  update(dt * m_ticksToSecond);
}

void Motor::update(const float elapsed) {
  m_time += elapsed;
  if (!m_inPosition) {
    float v = getV(m_time, m_Td);
    Driver::Direction dir = Driver::Forward;

    // Account for direction
    if (m_positionGoal < m_position) {
      v = -v;
      dir = Driver::Backward;
    }

    // Needed move in [unit]
    float dx = elapsed * v;
    m_position += dx;
    // Actual move in [step]
    int64_t dsteps = static_cast<int64_t>(round(dx * m_stepPerUnit));
    // Accumulated error in [unit]
    m_positionError += dx - (dsteps * m_unitPerStep);

    // Correct error ( under one [step] )
    if (fabs(m_positionError) > m_unitPerStep) {
      int sign = sgn(m_positionError);
      dsteps += sign;
      m_positionError -= sign * m_unitPerStep;
    }

    // Send steps to driver
    m_driver->step(dir, abs(dsteps));

    // In position
    if (m_time >= m_Td + m_Ts) {
      m_inPosition = true;
      m_positionLastGoal = m_positionGoal;
    }
  }
}

bool Motor::move(const float position) {
  if (!m_inPosition)
    return false;

  m_time = 0.f;
  m_positionGoal = position;
  m_inPosition = false;

  m_Td = fabs(m_positionGoal - m_position) / m_Vmax;
  return true;
}

bool Motor::done() {
  return m_inPosition;
}

float Motor::getPosition() {
  return m_position;
}

float Motor::getV(const float t, const float Td) {
  auto f = [&](const float t) {
    return m_Vmax * (1 - expf(-powf(m_alpha * t, 3)));
  };

  if (t < 0)
    return 0.f;

  if (t < Td)
    return f(t);
  else
    return f(t) - f(t - Td);
}

float Motor::getMaxAlpha() {
  return std::min(m_Amax / (1.1754f * m_Vmax),
                  sqrtf(m_Jmax / (2.1524f * m_Vmax)));
}

}  // namespace stepper
