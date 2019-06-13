//
// Created by Dmytro Babenko on 2019-06-11.
//

#ifndef KSVD_LINEARALGEBRAOPERATIONS_H
#define KSVD_LINEARALGEBRAOPERATIONS_H

#include "Matrix.h"

#include <vector>

class LAOperator
{
public:

    virtual float innerProduct(const std::vector<float>& v1, const std::vector<float>& v2);
    virtual std::vector<float> v1Minusv2(const std::vector<float>& v1, const std::vector<float >& v2);
    virtual float l2Norm(std::vector<float>& v);

    virtual Matrix matrixMultiplication(const Matrix& A, const Matrix& B);
    virtual Matrix AminusB(const Matrix& A, const Matrix& B);

    virtual void computeSVD(const Matrix& A, Matrix& U, Matrix& S, Matrix& Vt);


    virtual ~LAOperator() = default;

protected:
    virtual Matrix transposeMatrix(const Matrix& A);

};


#endif //KSVD_LINEARALGEBRAOPERATIONS_H
