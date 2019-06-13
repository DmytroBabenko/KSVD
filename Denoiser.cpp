//
// Created by Dmytro Babenko on 2019-06-11.
//

#include "Denoiser.h"
#include "SIMD_LAOperator.h"


Denoiser::Denoiser(bool useParallel, bool useSIMD)
: mLAOperator(useSIMD ?  std::make_shared<SIMD_LAOperator>() : std::make_shared<LAOperator>())
, ksvd(mLAOperator, useParallel)
{
}

Matrix Denoiser::denoise(const Matrix &noiseMatrix)
{
    const auto& Y = noiseMatrix;
    KSVDResult ksvdResult = ksvd.run(Y);

    const auto& D = ksvdResult.D;
    const auto& X = ksvdResult.X;

    Matrix denoisedY = mLAOperator->matrixMultiplication(X, D);

    return denoisedY;

}
