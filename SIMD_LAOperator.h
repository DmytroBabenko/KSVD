//
// Created by Dmytro Babenko on 2019-06-12.
//

#ifndef KSVD_SIMD_LAOPERATOR_H
#define KSVD_SIMD_LAOPERATOR_H

#include "LAOperator.h"

class SIMD_LAOperator : public  LAOperator{

    virtual float innerProduct(const std::vector<float>& v1, const std::vector<float>& v2) override ;
    virtual std::vector<float> v1Minusv2(const std::vector<float>& v1, const std::vector<float >& v2) override ;
    virtual float l2Norm(std::vector<float>& v) override ;

    virtual Matrix matrixMultiplication(const Matrix& A, const Matrix& B) override ;


};


#endif //KSVD_SIMD_LAOPERATOR_H
