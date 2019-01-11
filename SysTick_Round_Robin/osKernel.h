#ifndef _RTOS_H
#define _RTOS_H

#include <stdint.h>
#include "msp.h"

void rtosLaunch(uint32_t quanta);

void rtosInit(void);

uint8_t rtosAddThreads(void(*thread0)(void), void(*thread1)(void), void(*thread2)(void));

#endif