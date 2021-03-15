
#include "cubelet.h"

#define CONTROLER 22800

#define ROTATOR_POWER_UP 80
#define ROTATOR_POWER_DOWN 80

void setup() {
  block_value = 0;
}

void loop () {
  uint8_t instruction = get_block_value(CONTROLER);
  switch (instruction) {
    case 5:
      set_rotate_direction(FORWARD);
      set_rotate(ROTATOR_POWER_DOWN);
      break;
    case 6:
      set_rotate_direction(BACKWARD);
      set_rotate(ROTATOR_POWER_UP);
      break;
    default:
      set_rotate(0);
      break;
  }
}
