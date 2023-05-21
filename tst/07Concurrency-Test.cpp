#include "gtest/gtest.h"
#include <future>
#include <thread>

#include "07Concurrency.h"

TEST(ConcurrencyTestItem35, ThreadBasedTasks)
{
    std::size_t numTasks = 10;
    // no convenient way to get the return value from the thread
    createTasksThread(numTasks, addSleep);

    SUCCEED();
}

TEST(ConcurrencyTestItem35, ThreadBasedTasksLargeNumberOfThreads)
{
    auto tooManyThreads = []()
    {
        std::size_t numTasks = 50'000;
        createTasksThread(numTasks, add);
    };

    ASSERT_DEATH(tooManyThreads(), "terminate called");
}

TEST(ConcurrencyTestItem35, AsyncBasedTasks)
{
    std::size_t numTasks = 10;
    auto results = createTasksAsync(numTasks, addSleep);

    EXPECT_EQ(results.size(), numTasks);
    EXPECT_EQ(results[0], 0);
    EXPECT_EQ(results[1], 2);
}

TEST(ConcurrencyTestItem35, AsyncBasedTasksLargeNumber)
{
    std::size_t numTasks = 50'000;
    auto results = createTasksAsync(numTasks, add);

    SUCCEED();
}

TEST(ConcurrencyTestItem36, LaunchPolicyAsyncTask)
{
    auto task = asyncTask(add, 1, 2);

    auto policy = testLaunchPolicy(task);

    EXPECT_EQ(policy, LaunchPolicy::Async);
}

TEST(ConcurrencyTestItem36, LaunchPolicyDeferredTask)
{
    auto task = deferredTask(add, 1, 2);

    auto policy = testLaunchPolicy(task);

    EXPECT_EQ(policy, LaunchPolicy::Deferred);
}

TEST(ConcurrencyTestItem37, RAIIThreadJoin)
{
    bool isThreadExecuted = false;

    {
        std::thread t([&isThreadExecuted]() {
            isThreadExecuted = true;
        });

        ThreadRAII tr(std::move(t), DtorAction::Join);
        std::thread& thread = tr.get();

        EXPECT_TRUE(thread.joinable());
    }

    EXPECT_TRUE(isThreadExecuted);
}

TEST(ConcurrencyTestItem37, NonJoinableRAIIThread)
{
    std::thread t;

    {
        ThreadRAII tr(std::move(t), DtorAction::Join);
        std::thread& thread = tr.get();

        EXPECT_FALSE(thread.joinable());
    }

    SUCCEED();
}

TEST(ConcurrencyTestItem38, FuturePromise) {
    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    std::thread thread([&promise]() {
        int result = addSleep(2, 3);
        promise.set_value(result);
    });

    int result = future.get();

    thread.join();

    EXPECT_EQ(result, 5);
}

TEST(ConcurrencyTestItem38, PackagedTask) {
    std::packaged_task<int(int, int)> task([](int a, int b) {
        return add(a, b);
    });

    std::future<int> future = task.get_future();
    std::thread thread(std::move(task), 2, 3);

    int result = future.get();

    thread.join();

    EXPECT_EQ(result, 5);
}

TEST(ConcurrencyTestItem40, VolatileThreadUnsafe)
{
    constexpr int numTasks = 5000;

    auto result = incrementVolatile(numTasks);

    EXPECT_NE(result, numTasks);
}

TEST(ConcurrencyTestItem40, AtomicThreadSafe)
{
    constexpr int numTasks = 5000;

    auto result = incrementAtomic(numTasks);

    EXPECT_EQ(result, numTasks);
}