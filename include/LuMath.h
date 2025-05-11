#ifndef LU_MATH_H
#define LU_MATH_H

#include <inttypes.h>
#include <math.h>

#include "LuConstants.h"

#define LogN(base, num) (log(num) / log(base))

#define Lerp(start, end, t) ((start) + ((float)((end) - (start)) * (t)))

#define Min(a, b)                   \
	({                              \
		const __typeof__(a) a_ = a; \
		const __typeof__(b) b_ = b; \
		a_ < b_ ? a_ : b_;          \
	})

#define Max(a, b)                   \
	({                              \
		const __typeof__(a) a_ = a; \
		const __typeof__(b) b_ = b; \
		a_ > b_ ? a_ : b_;          \
	})

#define Clamp(val, lo, hi)                              \
	({                                                  \
		const __typeof__(val) val_ = val;               \
		const __typeof__(lo) lo_ = lo;                  \
		const __typeof__(hi) hi_ = hi;                  \
		(val_ > hi_ ? hi_ : (val_ < lo_ ? lo_ : val_)); \
	})

#define Tribool(left, right) ((right) - (left))

#define Index1D(x, y, width) ((x) + ((y) * (width)))

#define LuAnglePrecision (TWO_PI / (UINT16_MAX + 1))

#define f2angle(f)                             \
	({                                         \
		float f_ = fabsf((float)(f));          \
		f_ -= TWO_PI * (int64_t)(f_ / TWO_PI); \
                                               \
		(uint16_t)(f_ / LuAnglePrecision);     \
	})

#define LuCos(angle) SIN_COS_TABLE[(uint16_t)(angle)]
#define LuCosf(anglef) LuCos(f2angle(anglef))

#define LuSin(angle) SIN_COS_TABLE[(uint16_t)((angle) + ((UINT16_MAX + 1) / 4))]
#define LuSinf(anglef) LuSin(f2angle(anglef))

extern const float SIN_COS_TABLE[UINT16_MAX + 1];

#endif	// LU_MATH_H
