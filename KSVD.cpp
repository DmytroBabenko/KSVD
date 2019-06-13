//
// Created by Dmytro Babenko on 2019-06-11.
//

#include "KSVD.h"
#include "LAOperator.h"
#include "SIMD_LAOperator.h"

#include <list>
#include <ctime>
#include <cmath>
#include <future>


KSVDResult KSVD::run(const Matrix &Y)
{
    if (Y.empty())
        return KSVDResult();

    const size_t featureSize = Y.front().size();

    Matrix D = createInitDictionary(featureSize, K_ATOMS);

    for (size_t i = 0; i < NUM_ITER; ++i)
    {
        Matrix X = ompFun(Y, D);
        updateDictionary(D, X, Y);
    }

    Matrix X = orthogonalMatchingPursuitSingle(Y, D);

    return {D, X};
}

Matrix KSVD::createInitDictionary(size_t featureSize, size_t kAtoms)
{
    srand (time(0));
    Matrix D;
    D.reserve(kAtoms);

    for (size_t k = 0; k < kAtoms; ++k)
    {
        float norm2 = 0;
        std::vector<float > d(featureSize, 0.0f);
        for (size_t i = 0; i < featureSize; ++i)
        {
            d[i] = rand() % 255;
            norm2 += d[i] * d[i];
        }
        norm2 = std::sqrt(norm2);

        std::transform(d.begin(), d.end(), d.begin(), std::bind(std::divides<>(), std::placeholders::_1, norm2));

        D.push_back(std::move(d));
    }

    return D;
}

Matrix KSVD::orthogonalMatchingPursuitSingle(const Matrix &Y, const Matrix &D)
{
    const size_t targetSize = Y.size();
    Matrix X;
    X.reserve(targetSize);

    for (size_t i = 0; i < targetSize; ++i)
    {
        const auto& signal = Y[i];
        auto x = omp(signal, D);
        X.push_back(std::move(x));
    }

    return X;
}


Matrix KSVD::orthogonalMatchingPursuitParallel(const Matrix &Y, const Matrix &D)
{
    const size_t targetSize = Y.size();

    Matrix X;
    X.resize(targetSize);

    std::vector<std::pair<size_t , std::future<std::vector<float>>>> indexAndFutures;

    for (size_t i = 0; i < targetSize; ++i)
    {
        const auto& signal = Y[i];

        auto future = mThreadPool->addTaskToAsyncExec(&KSVD::omp, this, signal, D);
        indexAndFutures.emplace_back(i, std::move(future));
    }

    //wait for results
    for (auto& item : indexAndFutures)
    {
        size_t index = item.first;
        auto& future = item.second;

        auto x = future.get();

        X[index] = std::move(x);
    }

    return X;
}


void KSVD::updateDictionary(Matrix& D, Matrix& X, const Matrix& Y)
{
    for (size_t k = 0; k < K_ATOMS; ++k)
    {
        std::vector<size_t> relavantXIndices;
        for (size_t i = 0; i < X.size(); ++i)
        {
            if (X[i][k] != 0)
                relavantXIndices.push_back(i);
        }

        if (relavantXIndices.empty())
            continue;

        Matrix relevantX, relevantY;
        relevantX.reserve(relavantXIndices.size());
        relevantY.reserve(relavantXIndices.size());
        for (size_t relevantIndex : relavantXIndices)
        {
            relevantY.push_back(Y[relevantIndex]);

            //eliminate the contribution of the current atom
            auto x = X[relevantIndex];
            x[k] = 0.0f;
            relevantX.push_back(std::move(x));
        }
        Matrix XD = mLAOperator->matrixMultiplication(relevantX, D);
        Matrix E = mLAOperator->AminusB(relevantY, XD);

        Matrix U, S, Vt;
        mLAOperator->computeSVD(E, U, S, Vt);

        //update dictionary
        D[k] = Vt[0];
        std::transform(D[k].begin(), D[k].end(), D[k].begin(), std::bind(std::multiplies<>(), std::placeholders::_1, -1));

//        std::vector<float> coefficients = Vt[0];
//        std::transform(coefficients.begin(), coefficients.end(), coefficients.begin(), std::bind(std::multiplies<T>(), std::placeholders::_1, -S[0][0]));

        //update sparse matrix X
        for (size_t i = 0 ; i < relavantXIndices.size(); ++i)
        {
            X[relavantXIndices[i]][k] = -1 * -S[0][0] * U[0][i];
        }
    }
}


std::vector<float> KSVD::omp(const std::vector<float> &y, const Matrix &D)
{
    std::vector<float> x(K_ATOMS, 0.0f);
    auto r = y; // y_vec - projection y_vec to d_vec, minimum distance

    std::list<size_t> atoms;
    for (size_t k = 0; k < K_ATOMS; ++k)
        atoms.push_back(k);

    size_t countIteration = 0;
    while (countIteration < 5)
    {

        float maxInnerProduct = 0.0f;
//        std::vector<std::vector<float>> atomsWithMaxProjection;
//        std::vector<size_t> atomsWithMaxProjectionIndices;
        size_t atomIdxWithMaxProjection = 0;
        auto iterAtomWithMaxProjection = atoms.begin();

        for (auto iter = atoms.begin(); iter != atoms.end(); ++iter)
        {
            size_t  k = *iter;
            const auto& d = D[k];

            float innerProduct =  std::abs(mLAOperator->innerProduct(r, d));

            if (innerProduct > maxInnerProduct)
            {
                maxInnerProduct = innerProduct;
                atomIdxWithMaxProjection = k;
                iterAtomWithMaxProjection = iter;
            }
        }

        x[atomIdxWithMaxProjection] = maxInnerProduct;
        atoms.erase(iterAtomWithMaxProjection);

//        atomsWithMaxProjectionIndices.push_back(atomIdxWithMaxProjection);
//        atomsWithMaxProjection.push_back(D[atomIdxWithMaxProjection]);

//        std::vector<float> w = calculateWeights(atomsWithMaxProjection, y);

//        for(size_t i = 0; i < atomsWithMaxProjectionIndices.size(); ++i)
//        {
//            size_t kAtomIdx = atomsWithMaxProjectionIndices[i];
//            x[kAtomIdx] = w[i];
//        }

        std::vector<float> xd = D[atomIdxWithMaxProjection];
        std::for_each(xd.begin(), xd.end(), [&](float & val) {val *= maxInnerProduct;});
        r = mLAOperator->v1Minusv2(r, xd);

        if (mLAOperator->l2Norm(r) < THRESHOLD)
            return x;

        ++countIteration;
    }

    return x;
}

KSVD::KSVD(std::shared_ptr<LAOperator> laOperator, bool useParallel)
: mLAOperator(laOperator)
{
    mThreadPool = useParallel ? std::make_unique<ThreadPool>(NUM_THREADS) : nullptr;

    ompFun = [&, useParallel](const Matrix& Y, const Matrix& D) -> Matrix
            {
                if (useParallel)
                    return this->orthogonalMatchingPursuitParallel(Y, D);
                else
                    return this->orthogonalMatchingPursuitSingle(Y, D);
            };
}



