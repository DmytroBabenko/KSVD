//
// Created by Dmytro Babenko on 2019-06-12.
//

#include "SIMD_LAOperator.h"

#include <xmmintrin.h>
#include <immintrin.h>

#include <cmath>



float SIMD_LAOperator::innerProduct(const std::vector<float>& v1, const std::vector<float>& v2)
{
    if (v1.size() != v2.size())
        throw std::invalid_argument("The vectors have inconsistent size");

    size_t size = v1.size();

    const float* const a = v1.data();
    const float* const b = v2.data();

    __m128 rA,rB, rC;
    __m128 r0 = _mm_setzero_ps();

    for (unsigned i = 0; i < size; i+=4){
        rA = _mm_load_ps(&a[i]);
        rB = _mm_load_ps(&b[i]);

        rC = _mm_mul_ps(rA, rB);
        r0 = _mm_add_ps(r0, rC);
    }

    float* buffer = (float*)&r0;

    float sum = 0;
    for (unsigned i = 0; i < 4 && i < size; ++i)
    {
        sum += *(buffer + i);
    }

    return sum;
}

std::vector<float> SIMD_LAOperator::v1Minusv2(const std::vector<float> &v1, const std::vector<float> &v2)
{
    if (v1.size() != v2.size())
        throw std::invalid_argument("The vectors have inconsistent size");

    size_t size = v1.size();

    const float* a = v1.data();
    const float* b = v2.data();

    std::vector<float> result(size, 0.0f);

    float* res = result.data();


    __m128 rA,rB,rC, rD, rResult;

    for (size_t i = 0; i < size; i+=4){
        rA = _mm_load_ps(&a[i]);
        rB = _mm_load_ps(&b[i]);

        rResult = _mm_sub_ps(rA, rB);
        _mm_store_ps(&res[i],rResult);
    }

    return result;
}

float SIMD_LAOperator::l2Norm(std::vector<float> &v)
{
    const size_t size = v.size();
    const float* a = v.data();

    __m128 rA, rA2, rResult;
    __m128 r0 = _mm_setzero_ps();


    for (size_t i = 0; i < size; i+=4){
        rA = _mm_load_ps(&a[i]);

        rA2 = _mm_mul_ps(rA, rA);
        r0 = _mm_add_ps(r0, rA2);

    }

    float* buffer = (float*)&r0;

    float squareSum = 0;
    for (unsigned i = 0; i < 4 && i < size; ++i)
    {
        squareSum += *(buffer + i);
    }

    float l2Norm = std::sqrt(squareSum);

    return l2Norm;
}

Matrix SIMD_LAOperator::matrixMultiplication(const Matrix &A, const Matrix &B)
{
    const size_t aRows = A.size();
    const size_t aCols = A[0].size();

    const size_t bRows = B.size();
    const size_t bCols = B[0].size();

    if (aCols != bRows)
        throw std::invalid_argument("Dimensions of matrices are not consistent");

    const Matrix Bt = LAOperator::transposeMatrix(B);

    Matrix C(aRows, std::vector<float>(bCols, 0.0f));

    for (size_t i = 0 ; i < aRows; ++i)
    {
        for (size_t j = 0; j < bCols; ++j)
        {
            C[i][j] = innerProduct(A[i], Bt[j]);
        }
    }
    return C;
}
