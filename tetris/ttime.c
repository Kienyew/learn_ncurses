#include <time.h>
#include "ttime.h"

void ts_interval(const struct timespec* start, const struct timespec* end, struct timespec* dest) 
{
    if (end->tv_nsec < start->tv_nsec) {
        dest->tv_sec = end->tv_sec - start->tv_sec - 1;
        dest->tv_nsec = 1000000000L + (long long)end->tv_nsec - (long long)start->tv_nsec;
    } else {
        dest->tv_sec = end->tv_sec - start->tv_sec;
        dest->tv_nsec = end->tv_nsec - start->tv_nsec;
    }
}

long getgametime_ms() {
    struct timespec interval;
    struct timespec timenow;
    clock_gettime(CLOCK_REALTIME, &timenow);
    ts_interval(&gamestarttime, &timenow, &interval);
    return interval.tv_sec * 1000 + interval.tv_nsec / 1000000;
}