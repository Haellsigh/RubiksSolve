#ifndef STEPPER_DRIVER_H
#define STEPPER_DRIVER_H

#include <cstdint>

namespace stepper {

/*!
 * \brief The base driver class. Subclass this and implement the step function.
 */
class Driver {
 public:
  enum Direction { Forward = true, Backward = false };

  /*!
   * \brief Executes a single step in the direction.
   * \param direction The direction of movement.
   * \param n The number of steps.
   */
  virtual void step(Direction direction = Forward, uint32_t n = 1) = 0;
};

}  // namespace stepper

#endif  // STEPPER_DRIVER_H
