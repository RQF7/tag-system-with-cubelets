
#include "cubelet.h"

#define SENSOR_LEFT 83755

#define TRESHOLD_ONE 50

void setup() {
  block_value = 0;
}

void loop() {
  if (get_block_value(SENSOR_LEFT) < TRESHOLD_ONE) {
    set_flashlight(255);
  } else {
    set_flashlight(0);
  }
}
