#ifndef _H_TTIME
#define _H_TTIME

#include <time.h>

extern struct timespec gamestarttime;
void ts_interval(const struct timespec* start, const struct timespec* end, struct timespec* dest);
long getgametime_ms();

#endif
