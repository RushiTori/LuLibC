#include "LuMatrix.h"

#define MatrixGetFast(matrix, x, y) ((matrix)->data[Index1D(x, y, (matrix)->width)])
#define LuMatrixSize(matrix) ((matrix)->width * (matrix)->height)

LuMatrix* LuMatrixCreate(size_t width, size_t height) {
	LuMatrix* matrix = NULL;
	if (!Malloc(matrix, sizeof(LuMatrix))) return NULL;
	if (!Calloc(matrix->data, width * height, sizeof(float))) FreeAndReturn(matrix, NULL);

	LuMatrix matrixData = (LuMatrix){
		.data = matrix->data,
		.width = width,
		.height = height,
	};

	memcpy(matrix, &matrixData, sizeof(LuMatrix));

	return matrix;
}

LuMatrix* LuMatrixCopy(const LuMatrix* matrix) {
	LuMatrix* copy = LuMatrixCreate(matrix->width, matrix->height);
	if (!copy) return NULL;
	memcpy(copy->data, matrix->data, matrix->width * matrix->height * sizeof(float));
	return copy;
}

LuMatrix* LuMatrixGetIdentityMatrix(size_t n) {
	LuMatrix* matrix = LuMatrixCreate(n, n);
	if (!matrix) return NULL;

	for (size_t i = 0; i < n; i++) {
		MatrixGetFast(matrix, i, i) = 1;
	}

	return matrix;
}

void LuMatrixFree(LuMatrix* matrix) {
	Free(matrix->data);
	Free(matrix);
}

float* LuMatrixGetRef(const LuMatrix* matrix, size_t x, size_t y) { return matrix->data + Index1D(x, y, matrix->width); }

float LuMatrixGet(const LuMatrix* matrix, size_t x, size_t y) { return *LuMatrixGetRef(matrix, x, y); }

void LuMatrixSet(const LuMatrix* matrix, size_t x, size_t y, float value) { *LuMatrixGetRef(matrix, x, y) = value; }

bool LuMatrixIsSquare(const LuMatrix* matrix) { return (matrix->width == matrix->height); }

bool LuMatrixCanHadamard(const LuMatrix* a, const LuMatrix* b) { return ((a->width == b->width) && (a->height == b->height)); }

LuMatrix* LuMatrixAdd(LuMatrix* a, const LuMatrix* b) {
	if (LuMatrixCanHadamard(a, b)) {
		const size_t size = LuMatrixSize(a);
		for (size_t i = 0; i < size; i++) {
			a->data[i] += b->data[i];
		}
	}
	return a;
}

LuMatrix* LuMatrixSub(LuMatrix* a, const LuMatrix* b) {
	if (LuMatrixCanHadamard(a, b)) {
		const size_t size = LuMatrixSize(a);
		for (size_t i = 0; i < size; i++) {
			a->data[i] -= b->data[i];
		}
	}
	return a;
}

LuMatrix* LuMatrixHadamardProduct(LuMatrix* a, const LuMatrix* b) {
	if (LuMatrixCanHadamard(a, b)) {
		const size_t size = LuMatrixSize(a);
		for (size_t i = 0; i < size; i++) {
			a->data[i] *= b->data[i];
		}
	}
	return a;
}

LuMatrix* LuMatrixHadamardSquare(LuMatrix* matrix) { return LuMatrixHadamardProduct(matrix, matrix); }

LuMatrix* LuMatrixHadamardDiv(LuMatrix* a, const LuMatrix* b) {
	if (LuMatrixCanHadamard(a, b)) {
		const size_t size = LuMatrixSize(a);
		for (size_t i = 0; i < size; i++) {
			a->data[i] /= b->data[i];
		}
	}
	return a;
}

LuMatrix* LuMatrixScale(LuMatrix* matrix, float scale) {
	const size_t size = LuMatrixSize(matrix);
	for (size_t i = 0; i < size; i++) {
		matrix->data[i] *= scale;
	}
	return matrix;
}

LuMatrix* LuMatrixNegate(LuMatrix* matrix) {
	const size_t size = LuMatrixSize(matrix);
	for (size_t i = 0; i < size; i++) {
		matrix->data[i] *= -1.0f;
	}
	return matrix;
}

LuMatrix* LuMatrixSwapCols(LuMatrix* matrix, size_t x1, size_t x2) {
	for (size_t y = 0; y < matrix->height; y++) {
		float tmp = MatrixGetFast(matrix, x1, y);
		MatrixGetFast(matrix, x1, y) = MatrixGetFast(matrix, x2, y);
		MatrixGetFast(matrix, x2, y) = tmp;
	}
	return matrix;
}

LuMatrix* LuMatrixSwapRows(LuMatrix* matrix, size_t y1, size_t y2) {
	for (size_t x = 0; x < matrix->height; x++) {
		float tmp = MatrixGetFast(matrix, x, y1);
		MatrixGetFast(matrix, x, y1) = MatrixGetFast(matrix, x, y2);
		MatrixGetFast(matrix, x, y2) = tmp;
	}
	return matrix;
}

LuMatrix* LuMatrixTranspose(LuMatrix* matrix) {
	LuMatrix matrixData = (LuMatrix){
		.data = matrix->data,
		.width = matrix->height,
		.height = matrix->width,
	};

	for (size_t y = 0; y < matrix->height; y++) {
		for (size_t x = 0; x < matrix->width; x++) {
			float tmp = MatrixGetFast(matrix, x, y);
			MatrixGetFast(matrix, x, y) = MatrixGetFast(&matrixData, y, x);
			MatrixGetFast(&matrixData, y, x) = tmp;
		}
	}

	return matrix;
}

LuMatrix* LuMatrixMap(LuMatrix* matrix, void (*func)(float*)) {
	const size_t size = LuMatrixSize(matrix);
	for (size_t i = 0; i < size; i++) {
		func(matrix->data + i);
	}
	return matrix;
}

LuMatrix* LuMatrixTransform(LuMatrix* matrix, float (*func)(float)) {
	const size_t size = LuMatrixSize(matrix);
	for (size_t i = 0; i < size; i++) {
		matrix->data[i] = func(matrix->data[i]);
	}
	return matrix;
}

LuMatrix* LuMatrixDotProduct(const LuMatrix* a, const LuMatrix* b) {
	if (a->width != b->height) return NULL;

	LuMatrix* matrix = LuMatrixCreate(b->width, a->height);
	if (!matrix) return NULL;

	for (size_t y = 0; y < matrix->height; y++) {
		for (size_t x = 0; x < matrix->width; x++) {
			for (size_t n = 0; n < a->width; n++) {
				MatrixGetFast(matrix, x, y) += MatrixGetFast(a, n, y) * MatrixGetFast(b, x, n);
			}
		}
	}

	return matrix;
}

LuMatrix* LuMatrixDotSquare(const LuMatrix* matrix) {
	if (!LuMatrixIsSquare(matrix)) return NULL;
	return LuMatrixDotProduct(matrix, matrix);
}

LuMatrix* LuMatrixGetTranspose(const LuMatrix* matrix) {
	LuMatrix* copy = LuMatrixCopy(matrix);
	if (!copy) return NULL;

	return LuMatrixTranspose(copy);
}
