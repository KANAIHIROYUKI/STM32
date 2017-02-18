#ifndef BOARD_H
#define BOARD_H
/*******************************************
 * board for stm32f407vg ver1.0 2015/10/9
 *
 * [Dependency]
 *
 *
 *
 * [Note]
 *
 * [Change history]
 * ver1.0 2015/10/ 8 The first version.
 ******************************************/
#ifdef __cplusplus
 extern "C" {
#endif

#include "mcutime.h"

void stm32_software_reset();

int stm32_watchdog_setup();
void stm32_watchdog_cycle();

int board_setup();
char* board_whoami();


#ifdef __cplusplus
}
#endif
#endif//BOARD_H
