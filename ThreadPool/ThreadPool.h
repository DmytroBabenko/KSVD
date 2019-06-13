//
// Created by Dmytro Babenko on 2019-01-30.
//

#ifndef WORDCOUNTER_THREADPOOL_H
#define WORDCOUNTER_THREADPOOL_H


#include <cstdio>

#include "Worker.h"

class ThreadPool
{
public:
    explicit ThreadPool(size_t numThreads);
    ~ThreadPool();

    template<class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> addTaskToAsyncExec(F&& f, Args&&... args);

private:
    WorkerPtr findLeastBusyWorker();


private:
    std::vector<WorkerPtr> mWorkers;
};

template<class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type> ThreadPool::addTaskToAsyncExec(F&& f, Args&&... args)
{
    WorkerPtr worker = findLeastBusyWorker();
    return worker->addTask(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
}
#endif //WORDCOUNTER_THREADPOOL_H
