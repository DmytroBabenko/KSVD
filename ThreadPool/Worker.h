//
// Created by Dmytro Babenko on 2019-01-30.
//

#ifndef WORDCOUNTER_WORKER_H
#define WORDCOUNTER_WORKER_H

#include <atomic>
#include <mutex>
#include <thread>
#include <queue>
#include <future>
#include <functional>

class Worker
{
public:
    Worker();
    ~Worker();

    template<class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> addTask(F&& f, Args&&... args);

    size_t getNumberOfTasks() const;
    bool isFree()  const;

private:
    void addTaskToQueue(std::function<void()> task);
    void threadExec();

private:
    mutable std::mutex mMutex;
    std::atomic_bool mEnabled;
    std::condition_variable mCv;
    std::queue<std::function<void()>> mTasks;
    std::atomic_bool mIsRunning;
    std::thread mThread;

};

typedef std::shared_ptr<Worker> WorkerPtr;

template<class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type> Worker::addTask(F&& f, Args&&... args)
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >( std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    addTaskToQueue([task]()
    {
        (*task)();
    });

    return task->get_future();
}

#endif //WORDCOUNTER_WORKER_H
