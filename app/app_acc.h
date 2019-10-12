
#ifndef ALL_ACC_H
#define ALL_ACC_H

#define ACC_TRANSFER_CPLT_SIGNAL (1<<0)

#include "stdint.h"

union accData{
	uint8_t bytes[6];
	struct __attribute__((__packed__)) {
		int16_t x;
		int16_t y;
		int16_t z;
	}data;
};


void task_acc(void const * argument);

void acc_init();


#endif //ALL_ACC_H
