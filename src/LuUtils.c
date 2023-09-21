#include "LuUtils.h"

const string ProjectTitle = "Empty Project";

double getDeltaTimeNano(const Timer* start, const Timer* end) {
	return ((double)end->tv_sec * 1e9 + end->tv_nsec) - ((double)start->tv_sec * 1e9 + start->tv_nsec);
}

double getDeltaTime(const Timer* start, const Timer* end) { return getDeltaTimeNano(start, end) / 1e3; }

double getDeltaTimeMillis(const Timer* start, const Timer* end) { return getDeltaTimeNano(start, end) / 1e6; }

double getDeltaTimeSeconds(const Timer* start, const Timer* end) { return getDeltaTimeNano(start, end) / 1e9; }

double asbVal(double val){
	if(val<0) return -val;
	return val;
}
