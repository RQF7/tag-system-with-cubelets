
#include "cubelet.h"

#define SENSOR_CENTER 88373

#define TRESHOLD_ONE 50

void setup() {
  block_value = 0;
}

void loop() {
  if (get_block_value(SENSOR_CENTER) < TRESHOLD_ONE) {
    set_flashlight(255);
  } else {
    set_flashlight(0);
  }
}
