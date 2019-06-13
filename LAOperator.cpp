//
// Created by Dmytro Babenko on 2019-06-11.
//

#include "LAOperator.h"
#include "Utils.h"

#include <opencv2/opencv.hpp>


#include <cmath>

float LAOperator::innerProduct(const std::vector<float>& v1, const std::vector<float>& v2)
{
    if (v1.size() != v2.size())
        throw std::invalid_argument("No equal size of vectors");

    const size_t  size = v1.size();
    float product = 0.0f;

    for (auto iter1 = v1.cbegin(), iter2 = v2.begin();
            iter1 != v1.cend() && iter2 != v2.cend();
            ++iter1, ++iter2)
    {
        product += (*iter1) * (*iter2);
    }

    return product;
}

std::vector<float> LAOperator::v1Minusv2(const std::vector<float>& v1, const std::vector<float >& v2)
{
    if (v1.size() != v2.size())
        throw std::invalid_argument("No equal size of vectors");

    std::vector<float> result(v1.size());

    std::transform(v1.cbegin(), v1.cend(), v2.cbegin(), result.begin(), std::minus<float>());

    return result;

}

float LAOperator::l2Norm(std::vector<float>& v)
{
    float squareSum = 0.0f;
    std::for_each(v.cbegin(), v.cend(), [&] (float value)
    {
       squareSum +=  value * value;
    });

    return std::sqrt(squareSum);
}


Matrix LAOperator::matrixMultiplication(const Matrix& A, const Matrix& B)
{
    const size_t aRows = A.size();
    const size_t aCols = A[0].size();

    const size_t bRows = B.size();
    const size_t bCols = B[0].size();



    if (aCols != bRows)
        throw std::invalid_argument("Dimensions of matrices are not consistent");


    Matrix C(aRows, std::vector<float>(bCols, 0.0f));


    for (size_t i = 0; i < aRows; ++i)
    {
        for (size_t j = 0; j < aCols; ++j)
        {
            for (size_t k = 0; k < bCols; ++k)
            {
                C[i][k] += A[i][j] * B[j][k];
            }
        }
    }

    return C;

}


Matrix LAOperator::AminusB(const Matrix& A, const Matrix& B)
{
    if (A.size() != B.size())
        throw std::invalid_argument("No equal size of vectors");

    const size_t size = A.size();
    Matrix C(size);

    for (size_t i = 0 ; i < size; ++i)
    {
        C[i] = v1Minusv2(A[i], B[i]);
    }

    return C;
}


void LAOperator::computeSVD(const Matrix& A, Matrix& U, Matrix& S, Matrix& Vt)
{
    cv::Mat cvA = Utils::Matrix2cvMat(A);

    cv::Mat cvS,cvU,cvVt ;
    cv::SVD::compute(cvA, cvS, cvU, cvVt) ;

    U = Utils::cvMat2Matrix(cvU);
    S = Utils::cvMat2Matrix(cvS);
    Vt = Utils::cvMat2Matrix(cvVt);
}

Matrix LAOperator::transposeMatrix(const Matrix &A)
{
    size_t aRows = A.size();
    size_t aCols = A.front().size();

    Matrix At(aCols);

    for (size_t i = 0; i < aCols; ++i)
    {
        std::vector<float> row(aRows);
        for (size_t j = 0; j < aRows; ++j)
        {
            row[j] = A[j][i];
        }
        At[i] = std::move(row);
    }

    return At;
}
