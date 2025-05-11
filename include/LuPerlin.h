#ifndef LU_PERLIN_H
#define LU_PERLIN_H

#include "LuMath.h"

void SeedLuNoise(uint16_t seed);

uint32_t GetLuNoisePrecision();
void SetLuNoisePrecision(uint32_t precision);

float LuNoise3D(float x, float y, float z);
float LuNoise2D(float x, float y);

#endif	// LU_PERLIN_H
