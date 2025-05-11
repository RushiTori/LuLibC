#include "LuPerlin.h"

#include <stdlib.h>

#define PrintVar(var, fmt) printf("\t" #var ": " fmt "\n", var)

static uint16_t noiseSeed = 0;
static uint32_t octaves = 8;
static float RANDZ_TABLE[0x10000] = {0};
static float SQRTZ_TABLE[0x10000] = {0};

void SeedLuNoise(uint16_t newSeed) {
	noiseSeed = newSeed;
	for (uint32_t i = 0; i < 0x10000; i++) {
		RANDZ_TABLE[i] = (rand() / (float)RAND_MAX) * 2 - 1;
		SQRTZ_TABLE[i] = sqrtf(1 - (RANDZ_TABLE[i] * RANDZ_TABLE[i]));
	}
}

uint32_t GetLuNoisePrecision() { return octaves; }

void SetLuNoisePrecision(uint32_t precision) { octaves = precision; }

static inline uint32_t Hash2D(uint32_t x, uint32_t y) {
	const uint32_t s = 8 * sizeof(uint32_t) / 2;

	x *= 3284157443;
	y ^= (x << s) | (x >> s);
	y *= 1911520717;
	x ^= (y << s) | (y >> s);
	x *= 2048419325;
	return x;
}

static inline float Gradient3D(float x, float y, float z, int32_t ix, int32_t iy, int32_t iz) {
	const uint32_t a = Hash2D(Hash2D(ix, iy), iz) + noiseSeed;

	const float dx = x - ix;
	const float dy = y - iy;
	const float dz = z - iz;

	const uint16_t cosIdx = a & 0xFFFF;
	const uint16_t sinIdx = cosIdx - 0x3FFF;
	const uint16_t zIdx = Hash2D(Hash2D(ix, iz), iy) + noiseSeed;

	const float gradZ = RANDZ_TABLE[zIdx];
	const float zPart = SQRTZ_TABLE[zIdx];

	const float gradX = LuCos(cosIdx) * zPart;
	const float gradY = LuCos(sinIdx) * zPart;

	return ((dx * gradX) + (dy * gradY) + (dz * gradZ));
}

static inline float NoiseValue3D(float x, float y, float z) {
	int32_t x0 = (int32_t)x;
	int32_t x1 = x0 + ((x < 0) ? -1 : 1);
	float xDec = x - x0;
	if (xDec < 0) xDec = -xDec;
	float xT = xDec * xDec * (3 - xDec * 2);

	int32_t y0 = (int32_t)y;
	int32_t y1 = y0 + ((y < 0) ? -1 : 1);
	float yDec = y - y0;
	if (yDec < 0) yDec = -yDec;
	float yT = yDec * yDec * (3 - yDec * 2);

	int32_t z0 = (int32_t)z;
	int32_t z1 = z0 + ((z < 0) ? -1 : 1);
	float zDec = z - z0;
	if (zDec < 0) zDec = -zDec;
	float zT = zDec * zDec * (3 - zDec * 2);

	float topLeft = Lerp(Gradient3D(x, y, z, x0, y0, z0), Gradient3D(x, y, z, x1, y0, z0), xT);
	float topRight = Lerp(Gradient3D(x, y, z, x0, y1, z0), Gradient3D(x, y, z, x1, y1, z0), xT);
	float topLerp = Lerp(topLeft, topRight, yT);

	float bottomLeft = Lerp(Gradient3D(x, y, z, x0, y0, z1), Gradient3D(x, y, z, x1, y0, z1), xT);
	float bottomRight = Lerp(Gradient3D(x, y, z, x0, y1, z1), Gradient3D(x, y, z, x1, y1, z1), xT);
	float bottomLerp = Lerp(bottomLeft, bottomRight, yT);

	return Lerp(topLerp, bottomLerp, zT);
}

float LuNoise3D(float x, float y, float z) {
	float perlin = 0;
	float ampFreq = 1;

	for (uint32_t i = 0; i < octaves; i++) {
		perlin += NoiseValue3D(x * ampFreq, y * ampFreq, z * ampFreq) / ampFreq;
		ampFreq *= 2;
	}

	return Clamp(perlin, -1.0f, 1.0f);
}

static inline float Gradient2D(float x, float y, int32_t ix, int32_t iy) {
	const uint32_t a = Hash2D(ix, iy) + noiseSeed;

	const uint16_t cosIdx = a & 0xFFFF;
	const uint16_t sinIdx = cosIdx - 0x3FFF;

	const float dx = x - ix;
	const float dy = y - iy;

	return ((dx * LuCos(cosIdx)) + (dy * LuCos(sinIdx)));
}

static inline float NoiseValue2D(float x, float y) {
	int32_t x0 = (int32_t)x;
	int32_t x1 = x0 + ((x < 0) ? -1 : 1);
	float xDec = x - x0;
	if (xDec < 0) xDec = -xDec;
	float xT = xDec * xDec * (3 - xDec * 2);

	int32_t y0 = (int32_t)y;
	int32_t y1 = y0 + ((y < 0) ? -1 : 1);
	float yDec = y - y0;
	if (yDec < 0) yDec = -yDec;
	float yT = yDec * yDec * (3 - yDec * 2);

	float topLerp = Lerp(Gradient2D(x, y, x0, y0), Gradient2D(x, y, x1, y0), xT);
	float bottomLerp = Lerp(Gradient2D(x, y, x0, y1), Gradient2D(x, y, x1, y1), xT);

	return Lerp(topLerp, bottomLerp, yT);
}

float LuNoise2D(float x, float y) {
	float perlin = 0;
	float ampFreq = 1;

	for (uint32_t i = 0; i < octaves; i++) {
		perlin += NoiseValue2D(x * ampFreq, y * ampFreq) / ampFreq;
		ampFreq *= 2;
	}

	return Clamp(perlin, -1.0f, 1.0f);
}
