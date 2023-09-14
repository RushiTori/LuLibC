#ifndef LU_UTILS_H
#define LU_UTILS_H

#include "LuConstants.h"
#include "LuTypes.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define logN(base, num) (log(num) / log(base))

#define lerp(start, end, t) ((start) + ((t) * (float)((end) - (start))))
#define proportion(val, start, end) ((val) - (start)) / (float)((end) - (start))
#define mapValue(val, startA, endA, startB, endB) lerp(startB, endB, proportion(val, startA, endA))

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define clamp(val, min, max) (((val) < (min)) ? (min) : (((val) < (max)) ? (val) : (max)))

#define tribool(left, right) ((right) - (left))
#define index1D(x, y, width) ((x) + ((y) * (width)))

double getDeltaTimeNano(const Timer* start, const Timer* end);
double getDeltaTime(const Timer* start, const Timer* end);	// MicroSeconds
double getDeltaTimeMillis(const Timer* start, const Timer* end);
double getDeltaTimeSeconds(const Timer* start, const Timer* end);

double asbVal(double val);

#endif	// LU_UTILS_H
