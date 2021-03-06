#include "aircraft.h"


static struct 
{
	motor_option* motor;
	small_number length;
}* motors;
static small_number motor_count=0;
typedef enum { runing = 0, stop = 1, error = -1 } status_level;

void * check_inited(){
	if ( motors==NULL)return FALSE;
	else return TRUE;
}
void * check_motor_inited(small_number id){
	if (id > motors->length - 1)return FALSE;
	else return TRUE;
}

void init(small_number _motor_count) {
	motor_count = _motor_count;
	motors = malloc(sizeof(motors));
	memset(motors,0,sizeof(motors));
	motors->motor = malloc(sizeof(motor_option)*_motor_count);
	motors->length = _motor_count;
	for (small_number i = 0; i < motor_count; i++)
	{
		motors->motor[motor_count]->id=i;
		motors->motor[motor_count]->status = stop;
	}
	
}

status_level motor_status(small_number id) {
	check_inited();
	check_motor_inited(id);
return NULL;
}


int motor_pin(small_number id, small_number pin) {
	check_inited();
	check_motor_inited(id);
	motors->motor[id]->pin = pin;
	
	return TRUE;
}

int motor_run_init(small_number id) {//需更改
	if(!check_inited())return FALSE;
	for (int speed = 0; speed < 100;speed+=10){
		digitalWrite(motors->motor[id]->pin, HIGH);
		delayMicroseconds(500);
		delayMicroseconds(speed);
		digitalWrite(motors->motor[id]->pin, LOW);
		delayMicroseconds(20000 - 500 - speed);
	}
}

int motor_run_loop(small_number id) {
	if(!check_inited() && !check_motor_inited(id))return FALSE;
	motors->motor[id]->status = runing;
	while (motors->motor[id]->status==runing){
		digitalWrite(motors->motor[id]->pin, HIGH);
		delayMicroseconds(500);
		delayMicroseconds(motors->motor[id]->speed);
		digitalWrite(motors->motor[id]->pin, LOW);
		delayMicroseconds(20000-500-motors->motor[id]->speed);
	};
	return TRUE;
}


int *get_motor_speed_pointer(small_number id) {
	if (id > motors->length - 1)return FALSE;
	return &motors->motor[id]->speed;
}
