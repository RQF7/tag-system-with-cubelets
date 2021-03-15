
#include "cubelet.h"

#define SENSOR_LEFT  1368089
#define SENSOR_CENTER 88467
#define SENSOR_RIGHT 1368072

#define TRESHOLD_ONE 50
#define TRESHOLD_TWO 100
#define TIME_TO_UP_LEAVER 4000

int internal_state;
int last_symbol_readed;
int erasing_value;
int counter_of_erased_symbols;
int counter_of_symbols;
int counter_of_zero_symbols;
int counter_of_one_symbols;
int new_position;

int test_position() {

  if (get_block_value(SENSOR_LEFT) < TRESHOLD_ONE)
    return 1;

  if (get_block_value(SENSOR_CENTER) < TRESHOLD_ONE)
    return 2;

  if (get_block_value(SENSOR_RIGHT) < TRESHOLD_ONE)
    return 3;

  return 0;
}

void computation() {
  block_value = 0;
  wait(500);
  uint8_t position = test_position();
  if (position == 2)
    last_symbol_readed = 0;
  else if (position == 3)
    last_symbol_readed = 1;
  internal_state = 3;
}

void setup() {
  block_value = 0;
  internal_state = 0;
  erasing_value = 3;
  counter_of_erased_symbols = 0;
  counter_of_symbols = 0;
  counter_of_zero_symbols = 0;
  counter_of_one_symbols = 0;
  new_position = 0;
  set_bar_graph(0);
}

void loop() {
  switch (internal_state) {

    /* Initial delay */
    case 0:
      wait(5000);
      internal_state = 1;
      break;

    /* Forward until block */
    case 1:
      block_value = 1;
      if (test_position() != 0) {
        internal_state = 2;
      }
      break;

    /* Computation */
    case 2:
      computation();
      break;

    /* States of erasing mechanery ******************************************/

    /* Move right leaver until block in left position. */
    case 3:
      block_value = 5;
      if (test_position() == 1) {
        internal_state = 4;
      }
      break;

    /* Move left leaver to original position. */
    case 4:
      block_value = 6;
      wait(TIME_TO_UP_LEAVER);
      counter_of_erased_symbols++;
      internal_state = 5;
      break;

    /* Advance until out of block. */
    case 5:
      block_value = 1;
      if (test_position() == 0)
        internal_state = 6;
      break;

    /* Advance until new block */
    case 6:
      block_value = 1;
      if (test_position() != 0)
        internal_state = 7;
      break;

    /* Control of loop. */
    case 7:
      block_value = 0;
      wait(500);
      if (counter_of_erased_symbols == erasing_value) {
        counter_of_erased_symbols = 0;
        internal_state = 10;
      } else {
        internal_state = 3;
      }
      break;

    /* States for traversing the tape. **************************************/

    /* Control of loop. */
    case 10:
      block_value = 0;
      if (test_position() == 1) {
        if (last_symbol_readed == 0){
          internal_state = 20;
          counter_of_symbols += 2;
        } else {
          internal_state = 30;
          counter_of_symbols += 4;
        }
      } else {
        internal_state = 11;
        counter_of_symbols++;
      }
      break;

    /* Advance until out of block. */
    case 11:
      block_value = 1;
      if (test_position() == 0)
        internal_state = 12;
      break;

    /* Advance until new block. */
    case 12:
      block_value = 1;
      if (test_position() != 0) {
        internal_state = 10;
      }
      break;

    /* States for executing the zero production rule. ***********************/

    /* Move left leaver until position 2. */
    case 20:
      block_value = 3;
      if (test_position() == 2)
        internal_state = 21;
      break;

    /* Move left leaver to original position. */
    case 21:
      block_value = 4;
      wait(TIME_TO_UP_LEAVER);
      internal_state = 22;
      break;

    /* Control loop. */
    case 22:
      block_value = 0;
      counter_of_zero_symbols++;
      if (counter_of_zero_symbols == 2) {
        counter_of_zero_symbols = 0;
        internal_state = 40;
      } else {
        internal_state = 23;
      }
      break;

    /* Advance until out of block. */
    case 23:
      block_value = 1;
      if (test_position() == 0) {
        internal_state = 24;
      }
      break;

    /* Advance until new block. */
    case 24:
      block_value = 1;
      if (test_position() != 0) {
        internal_state = 20;
      }
      break;

    /* States for executing the one production rule. ************************/

    /* Determine new symbol to write. */
    case 30:
      block_value = 0;
      if (counter_of_one_symbols == 0)
        new_position = 3;
      else if (counter_of_one_symbols == 1)
        new_position = 3;
      else if (counter_of_one_symbols == 2)
        new_position = 2;
      else if (counter_of_one_symbols == 3)
        new_position = 3;
      internal_state = 31;
      break;

    /* Move left leaver until new position. */
    case 31:
      block_value = 3;
      if (test_position() == new_position)
        internal_state = 32;
      break;

    /* Move left leaver to original position. */
    case 32:
      block_value = 4;
      wait(TIME_TO_UP_LEAVER);
      internal_state = 33;
      break;

    /* Control of loop. */
    case 33:
      block_value = 0;
      counter_of_one_symbols++;
      if (counter_of_one_symbols == 4) {
        counter_of_one_symbols = 0;
        internal_state = 40;
      } else {
        internal_state = 34;
      }
      break;

    /* Advance until out of block. */
    case 34:
      block_value = 1;
      if (test_position() == 0) {
        internal_state = 35;
      }
      break;

    /* Advance until new block. */
    case 35:
      block_value = 1;
      if (test_position() != 0) {
        internal_state = 30;
      }
      break;

    /* States to return to the left of the tape. ****************************/

    /* Backward until out of block. */
    case 40:
      block_value = 2;
      set_bar_graph(15);
      if (test_position() == 0) {
        internal_state = 41;
      }
      break;

    /* Backward until new block. Control of loop. */
    case 41:
      block_value = 2;
      set_bar_graph(0);
      int position = test_position();
      if (position != 0) {
        if (position == 1) {
          internal_state = 42;
        } else {
          internal_state = 40;
        }
      }
      break;

    case 42:
      block_value = 1;
      if (test_position() == 0) {
        internal_state = 1;
      }
      break;

  }
}
