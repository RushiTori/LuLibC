#ifndef LU_MATRIX_H
#define LU_MATRIX_H

#include "LuMath.h"
#include "LuMemSafety.h"

typedef struct LuMatrix {
	float* data;
	const size_t width;
	const size_t height;
} LuMatrix;

LuMatrix* LuMatrixCreate(size_t width, size_t height);
LuMatrix* LuMatrixCopy(const LuMatrix* matrix);
LuMatrix* LuMatrixGetIdentityMatrix(size_t n);

void LuMatrixFree(LuMatrix* matrix);

float* LuMatrixGetRef(const LuMatrix* matrix, size_t x, size_t y);
float LuMatrixGet(const LuMatrix* matrix, size_t x, size_t y);
void LuMatrixSet(const LuMatrix* matrix, size_t x, size_t y, float value);

bool LuMatrixIsSquare(const LuMatrix* matrix);
bool LuMatrixCanHadamard(const LuMatrix* a, const LuMatrix* b);
float LuMatrixDeterminant(const LuMatrix* matrix);

// These functions will simply return the first non-const matrix passed as arg

LuMatrix* LuMatrixAdd(LuMatrix* a, const LuMatrix* b);
LuMatrix* LuMatrixSub(LuMatrix* a, const LuMatrix* b);

LuMatrix* LuMatrixHadamardProduct(LuMatrix* a, const LuMatrix* b);
LuMatrix* LuMatrixHadamardSquare(LuMatrix* matrix);
LuMatrix* LuMatrixHadamardDiv(LuMatrix* a, const LuMatrix* b);

LuMatrix* LuMatrixScale(LuMatrix* matrix, float scale);
LuMatrix* LuMatrixNegate(LuMatrix* matrix);

LuMatrix* LuMatrixSwapCols(LuMatrix* matrix, size_t x1, size_t x2);
LuMatrix* LuMatrixSwapRows(LuMatrix* matrix, size_t y1, size_t y2);
LuMatrix* LuMatrixTranspose(LuMatrix* matrix);

LuMatrix* LuMatrixMap(LuMatrix* matrix, void (*func)(float*));
LuMatrix* LuMatrixTransform(LuMatrix* matrix, float (*func)(float));

// These functions will attempt to create a new Matrix that you have to Free

LuMatrix* LuMatrixDotProduct(const LuMatrix* a, const LuMatrix* b);
LuMatrix* LuMatrixDotSquare(const LuMatrix* matrix);

LuMatrix* LuMatrixGetTranspose(const LuMatrix* matrix);

#endif	// LU_MATRIX_H
