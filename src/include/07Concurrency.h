#pragma once

#include <thread>
#include <future>
#include <chrono>
#include <vector>

/*
 * Item 35: Prefer task-based programming to thread-based
 */

int addSleep(int a, int b) 
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return a + b;
}
int add(int a, int b) 
{
    return a + b;
}

template <typename T>
void createTasksThread(std::size_t numTasks, T task) 
{
    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < numTasks; ++i) 
    {
        threads.emplace_back(task, i, i);
    }
    // impossible to get the return value from the thread
    for (auto& t : threads) 
    {
        t.join();
    }
}

template <typename T>
std::vector<int> createTasksAsync(std::size_t numTasks, T task)
{
    std::vector<std::future<int>> futures;
    for (std::size_t i = 0; i < numTasks; ++i) 
    {
        futures.emplace_back(std::async(task, i, i));
    }
    std::vector<int> results;
    for (auto& f : futures) 
    {
        results.push_back(f.get());
    }
    return results;
}

/*
 * Item 36: Specify std::launch::async if asynchronicity is essential
 */

enum class LaunchPolicy { Async, Deferred };

template<typename F, typename... Ts>
inline auto asyncTask(F&& f, Ts&&... params)
{
    return std::async(std::launch::async,
                      std::forward<F>(f),
                      std::forward<Ts>(params)...);
}

template<typename F, typename... Ts>
inline auto deferredTask(F&& f, Ts&&... params)
{
    return std::async(std::launch::deferred,
                      std::forward<F>(f),
                      std::forward<Ts>(params)...);
}

template<typename T>
LaunchPolicy testLaunchPolicy(T&& task)
{
    using namespace std::literals;
    if (task.wait_for(0s) == std::future_status::deferred)
    {
        return LaunchPolicy::Deferred;
    } 
    else 
    { 
        while (task.wait_for(100ms) != std::future_status::ready) 
        {
            // do something else
        }
        return LaunchPolicy::Async;
    }
}

/*
 * Item 37: Make std::threads unjoinable on all paths
 */

enum class DtorAction { Join, Detach };

class ThreadRAII 
{
    DtorAction action;
    std::thread t;
public:
    ThreadRAII(std::thread&& t, DtorAction a) : action(a), t(std::move(t)) {}
    ~ThreadRAII() 
    {
        if (t.joinable()) 
        {
            if (action == DtorAction::Join) 
            {
                t.join();
            } 
            else 
            {
                t.detach();
            }
        }
    }

    std::thread& get() { return t; }

    ThreadRAII(const ThreadRAII&) = delete;
    ThreadRAII& operator=(const ThreadRAII&) = delete;
    ThreadRAII(ThreadRAII&&) = default;
    ThreadRAII& operator=(ThreadRAII&&) = default;
};

/*
 * Item 40: Use std::atomic for concurrency, volatile for special memory
 */

int incrementVolatile(int numTasks)
{
    volatile int v = 0;
    
    std::promise<void> p;
    std::future<void> f = p.get_future();

    auto increment = [&v]() {
        v = v + 1;
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    };

    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < numTasks; ++i) 
    {
        threads.emplace_back(increment);
    }

    p.set_value();

    for (auto& t : threads) 
    {
        t.join();
    }

    return v;
}

int incrementAtomic(int numTasks)
{
    std::atomic<int> v = 0;
    
    std::promise<void> p;
    std::future<void> f = p.get_future();

    auto increment = [&v]() {
        ++v;
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    };

    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < numTasks; ++i) 
    {
        threads.emplace_back(increment);
    }

    p.set_value();

    for (auto& t : threads) 
    {
        t.join();
    }

    return v;
}