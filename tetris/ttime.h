#include <time.h>

struct timespec _gamestarttime;
void ts_interval(const struct timespec* start, const struct timespec* end, struct timespec* dest);
long getgametime_ms();