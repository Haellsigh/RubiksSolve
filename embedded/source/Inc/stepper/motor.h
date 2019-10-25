#ifndef STEPPER_MOTOR_HPP
#define STEPPER_MOTOR_HPP

#include <memory>

namespace stepper {

class Driver;

/*!
 * \brief The Motor class
 * \note The
 */
class Motor {
 public:
  /*!
   * \brief Initializes the motor.
   * \param Vmax Maximum velocity in [unit].
   * \param Amax Maximum acceleration in [unit/s].
   * \param Jmax Maximum jerk in user [unit/s^2].
   */
  Motor(const float Vmax, const float Amax, const float Jmax);

  /*!
   * \brief Sets the motor driver.
   * \param driver Pointer to the stepper driver.
   */
  void setDriver(Driver* driver);

  /*!
   * \brief Updates the motor characteristics
   * \param Vmax Maximum velocity in [unit].
   * \param Amax Maximum acceleration in [unit/s].
   * \param Jmax Maximum jerk in user [unit/s^2].
   */
  void setCharacteristics(const float Vmax, const float Amax, const float Jmax);

  /*!
   * \brief Sets the number of [steps] per [unit].
   * \param factor Conversion factor in [steps/unit]
   */
  void setStepPerUnit(const float factor);

  /*!
   * \brief Updates the motor, generating trajectories and updating the driver.
   * \param elapsed Elapsed time in seconds since last update.
   */
  void update(const float elapsed);
  void update();

  /*!
   * \brief Moves the motor to position.
   * \param position The position in [unit].
   * \retval true The move was scheduled.
   * \retval false The motor is already moving.
   */
  bool move(const float position);

  /**
   * @brief Done status.
   * @return Whether the movements are done.
   */
  bool done();

  float getPosition();

 private:
  float getV(const float t, const float Td);
  float getMaxAlpha();

 private:
  // Parameters
  float m_Vmax, m_Amax, m_Jmax;
  float m_stepPerUnit = 0.f;  // [step/unit]
  float m_unitPerStep = 0.f;  // [unit/step]

  // Calculated parameters
  float m_alpha, m_Ts;

  // Yes
  bool m_inPosition = true;
  float m_time = 0.f;
  float m_positionGoal = 0.f, m_positionLastGoal = 0.f;
  float m_Td = 0.f, m_position = 0.f, m_positionError = 0.f;

  uint32_t m_lastTick;
  float m_ticksToSecond;

  Driver* m_driver;
};

}  // namespace stepper

#endif  // STEPPER_MOTOR_HPP
