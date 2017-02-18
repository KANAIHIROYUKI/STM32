#ifndef MCUTIME_H
#define MCUTIME_H
/*******************************************
 * mcutime ver3.0 2015/10/9
 *
 * [Dependency]
 * stdint
 * layer_driver/board/board.h
 *
 * [Note]
 *
 * [Change history]
 * ver3.0 2015/10/ 9 improve dependency.  move definitions of micros()
 *                   and timer_setup() to board.c
 * ver2.0 2015/ 3/ 6 Wait inline.Rename time to mcutime.Time handle by int64_t.
 * ver1.0 2015/ 3/ 3 The first version.
 ******************************************/
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

 int64_t micros();
 void timer_setup();

 int64_t millis();
 void delay_ms(int64_t time);
 void wait(int64_t wait_time);

#ifdef __cplusplus
}
#endif

#endif
