#include "osKernel.h"
#include <stdint.h>
#include "msp.h"

void osSchedulerLaunch(void);

#define NUM_THREADS 3
#define STACK_SIZE 100
#define BUS_FREQUENCY 3000000
uint32_t MS_PRESCALER ;

struct tcb{
	int32_t *stackPointer;
	struct tcb *next;
};

typedef struct tcb tcbType;
tcbType tcbs[NUM_THREADS];
tcbType *current;

int32_t TCB_STACK[NUM_THREADS][STACK_SIZE];

void rtosStackInit(int i){
	tcbs[i].stackPointer = &TCB_STACK[i][STACK_SIZE - 16];
	TCB_STACK[i][STACK_SIZE-1] = 0x01000000;
}

uint8_t rtosAddThreads(void(*thread0)(void), void(*thread1)(void), void(*thread2)(void)){

	//Disable Interrupt so add thread process is atomic
	__disable_irq();
	
	//Link threads together in a circular linked list
	tcbs[0].next = &tcbs[1];
	tcbs[1].next = &tcbs[2];
	tcbs[2].next = &tcbs[0];
	
	//Initialize Stack for thread0 
	rtosStackInit(0);
	
	//Assign location of thread0 to PC of thread0 stack
	TCB_STACK[0][STACK_SIZE-2] = (int32_t)(thread0);
	
	//Initialize Stack for thread1
	rtosStackInit(1);
	
	//Assign location of thread1 to PC of thread1 stack
	TCB_STACK[1][STACK_SIZE-2] = (int32_t)(thread1);
	
	//Initialize Stack for thread2 
	rtosStackInit(2);
	
	//Assign location of thread2 to PC of thread2 stack
	TCB_STACK[2][STACK_SIZE-2] = (int32_t)(thread2);
	
	//Point to starting point of os
	current = &tcbs[0];
	
	//Re-enable interrupts
	__enable_irq();
	
	return 1;
}

void rtosInit(void){
	__disable_irq();
	
	MS_PRESCALER = (BUS_FREQUENCY / 1000);
	
}

void rtosLaunch(uint32_t quanta){
	
	//Initialize SysTick
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
	
	//Set SysTick Priority to lowest priority
	NVIC_SetPriority(SysTick_IRQn, 7);
	
	SysTick->LOAD = (quanta * MS_PRESCALER) - 1;
	
	//Enable SysTick
	SysTick->CTRL = 0x00000007;

	osSchedulerLaunch();
}
