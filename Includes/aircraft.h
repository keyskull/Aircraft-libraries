#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<Arduino.h>
typedef struct _Hardware_info Hardware_info;

/*-------------Basic------------------------*/
#define TRUE ((void*)0)
#define FALSE ((void*)-1)
typedef unsigned char small_number;
typedef int16_t __int16;

/*-------------Basic------------------------*/

/*-------------Memory-Manager-----------------------*/
typedef struct {
	void * pointer;
	void * protect_level;
}mem_table;
void *mem_apply(size_t);
void *mem_apply_important(size_t);
void *mem_memset(void *_Dst, int _Val, size_t _Size);
void mem_free(void *);
#define malloc(_Size) mem_apply(_Size)
#define memset(_Dst,_Val,_Size) mem_memset(_Dst,_Val,_Size)
#define malloc_important(_Size) mem_apply_important(_Size)
#define free(Pointer) mem_free(Pointer)
/*-------------Memory-Manager-----------------------*/

/*-------------Info-------------------------*/
typedef enum { INFO, WARRING, ERROR } msg_level;
void Message(msg_level,const char *_Format,...);
#define DEBUG_MSG_INFO(str,arge) \
 	Message(__DATE__" "__TIME__"INFO:"str,##arge)
#define DEBUG_MSG_WARRING(str,arge) \
 	Message(__DATE__" "__TIME__"INFO:"str,##arge)
#define DEBUG_MSG_ERROR(str,arge) \
 	Message(__DATE__" "__TIME__"INFO:"str,##arge)

/*-------------Info-------------------------*/


/*
*
*
*
*
*
*
*/

/*-------------driver-------------------------*/
typedef struct
{
	small_number id;
	char *Driver_name;
	char *Driver_info;
	struct { char * name; void* next; }*Parameter_name;
	small_number Parameter_name_count;
	Hardware_info* (*Search_Hardware)(void);
}Driver;

/*-------------driver-------------------------*/



/*-------------Hardware-------------------------*/
typedef struct _Hardware_info{
	small_number Hardware_id;
	Driver Driver_info;
	struct { char * name; __int16 value; void* next; }* Hardware_Data;
} Hardware_info;

void *Update_Hareware_info(void);
Hardware_info *Get_Hareware_info(void);
/*-------------Hardware-------------------------*/

/*------------------Motor-----------------------*/
typedef enum { runing = 0, stop = 1, error = -1 } status_level;
typedef float Motor_Speed ;
typedef	struct
{
	small_number id;
	small_number pin;
	status_level status;
	unsigned int speed;
}*motor_option;


/*------------------Motor----------------------*/

/*-----------飞行器算法优先级-------------------*/

/*-----------飞行器算法优先级-------------------*/

