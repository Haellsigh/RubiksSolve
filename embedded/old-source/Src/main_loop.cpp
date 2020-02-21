#include "main_loop.h"

#include <main.h>
#include <stepper/motor.h>
#include <stepper/stepdirdriver.h>

// void* cdc = CDC::self();

/*#include "stepper/motor.h"
#include "stepper/stepdirdriver.h"*/

void main_loop() {
  const float max_rmp = 250;
  float tau = 0.0001f;

  const float Vmax = max_rmp * 360.f / 60.f;  // rpm to deg/s
  float Amax = Vmax * 2.f / (5.f * tau);      // deg/s^2
  float Jmax = Amax * 2.f / (5.f * tau);      // deg/s^3

  stepper::Motor motor(Vmax, Amax, Jmax);
  stepper::StepDirDriver driver(DIR_GPIO_Port, DIR_Pin, STEP_GPIO_Port,
                                STEP_Pin);

  motor.setDriver(&driver);
  motor.setStepPerUnit(1600.f / 360.f);  // 1600 µstep / 360°

  float pos;

  float seq[] = {90, 180, 270, 360, 270, 180, 90, 0};
  int n = sizeof(seq) / sizeof(seq[0]);
  int i = 0;
  int k = 0;

  while (true) {
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
      if (motor.done()) {
        tau = 1.f / (10.f + 4 * k);
        Amax = Vmax * 2.f / (5.f * tau);
        Jmax = Amax * 2.f / (5.f * tau);

        motor.setCharacteristics(Vmax, Amax, Jmax);

        pos = seq[i];
        motor.move(pos);
        i++;
        k++;
        if (i >= n) {
          i = 0;
        }
      }
    }

    /*if (motor.done()) {
      pos = seq[i];
      motor.move(pos);
      i++;
      if (i >= n) {
        i = 0;
      }
    }*/

    motor.update();
  }
}
