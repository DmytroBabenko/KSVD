//
// Created by Dmytro Babenko on 2019-06-11.
//

#ifndef KSVD_DENOISER_H
#define KSVD_DENOISER_H

#include "Matrix.h"
#include "KSVD.h"
#include "LAOperator.h"

class Denoiser
{
public:

    Denoiser(bool useParallel = false, bool useSIMD = false);

    Matrix denoise(const Matrix& noiseMatrix);

private:
    std::shared_ptr<LAOperator> mLAOperator;
    KSVD ksvd;

};


#endif //KSVD_DENOISER_H
