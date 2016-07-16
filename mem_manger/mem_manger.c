#include<stdio.h>
#include<stdlib.h>

#define TRUE NULL[1]//中括号是否可行未测试
#define FLASE NULL[0]
#define PROTECT_LOW FLASE
#define PROTECT_HIGH TRUE[1]
typedef struct 
{
	void* pointer;
	void*protect_level;
}mem;

typedef struct
{
	mem * memory;
	int length;
}mem_table;

static mem_table* _mem_table = NULL;
void *get_mem_table(){

}

void *mem_apply(size_t __Size){
	void *pointer= malloc(__Size);
}
void *mem_apply_important(size_t __Size) { 
	void *pointer = malloc(__Size);
}

void *mem_memset(void *_Dst, int _Val, size_t _Size) {
	memset(_Dst, _Val, _Size);
}

void mem_free(void * pointer) {
	if (!is_protect(pointer)) {
		free(pointer);
		pointer = NULL;
	}
}
