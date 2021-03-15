
#include "cubelet.h"

#define CONTROLER 22800

#define MOTOR_POWER 64

void setup() {
  block_value = 0;
}

void loop() {
  uint8_t instruction = get_block_value(CONTROLER);
  switch (instruction) {
    case 1:
      set_drive_direction(FORWARD);
      set_drive(MOTOR_POWER);
      break;
    case 2:
      set_drive_direction(BACKWARD);
      set_drive(MOTOR_POWER);
      break;
    default:
      set_drive(0);
      break;
  }
}
