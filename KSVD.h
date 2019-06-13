//
// Created by Dmytro Babenko on 2019-06-11.
//

#ifndef KSVD_KSVD_H
#define KSVD_KSVD_H

#include "ThreadPool.h"

#include <vector>
#include <memory>

//forward declaration
class LAOperator;

typedef std::vector<std::vector<float>> Matrix;


struct KSVDResult
{
    Matrix D;
    Matrix X;
};

class KSVD
{
public:

    KSVD(std::shared_ptr<LAOperator> laOperator, bool useParallel);

    KSVDResult run(const Matrix& Y);

    Matrix orthogonalMatchingPursuitSingle(const Matrix &Y, const Matrix &D);
    Matrix orthogonalMatchingPursuitParallel(const Matrix &Y, const Matrix &D);

    void updateDictionary(Matrix& D, Matrix& X, const Matrix& Y);

private:

    Matrix createInitDictionary(size_t featureSize, size_t kAtoms);
    std::vector<float> omp(const std::vector<float>& y, const Matrix& D);

private:
    std::unique_ptr<ThreadPool> mThreadPool;
    std::shared_ptr<LAOperator> mLAOperator;
    std::function<Matrix(const Matrix&, const Matrix&)> ompFun;


private:
    static constexpr size_t NUM_THREADS = 8;
    static constexpr size_t K_ATOMS = 256;
    static constexpr size_t NUM_ITER = 10;
    static constexpr float THRESHOLD = 0.001;

};


#endif //KSVD_KSVD_H
