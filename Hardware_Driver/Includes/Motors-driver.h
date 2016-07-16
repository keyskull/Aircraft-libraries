#include "aircraft.h"



void init(small_number _motor_count);
status_level motor_status(small_number id);
int motor_pin(small_number id, small_number pin);
int motor_run_init(small_number id);
int motor_run_loop(small_number id);
int get_motor_max_low_speed(small_number id);
int *get_motor_speed_pointer(small_number id);
