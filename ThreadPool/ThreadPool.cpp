//
// Created by Dmytro Babenko on 2019-01-30.
//

#include "ThreadPool.h"


ThreadPool::ThreadPool(size_t numThreads)
{
    if (numThreads == 0)
        numThreads = 1;

    mWorkers.reserve(numThreads);
    for (size_t i = 0; i < numThreads; ++i)
    {
        mWorkers.emplace_back(std::make_shared<Worker>());
    }
}

ThreadPool::~ThreadPool()
{
}


WorkerPtr ThreadPool::findLeastBusyWorker()
{
    WorkerPtr worker = nullptr;

    size_t minTask = UINT32_MAX;
    for (auto& pWorker : mWorkers)
    {
        if (pWorker->isFree())
            return pWorker;
        size_t numOfTask = pWorker->getNumberOfTasks();
        if (minTask > numOfTask)
        {
            minTask = numOfTask;
            worker = pWorker;
        }
    }
    return worker;
}


