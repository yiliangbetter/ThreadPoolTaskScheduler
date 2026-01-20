#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

namespace ThreadPool {

class ThreadPool {
public:
  explicit ThreadPool(size_t numThreads);
  ~ThreadPool();

  template <typename F, typename... Args>
  std::future<std::invoke_result_t<F, Args...>> submit(F &&f, Args &&...args) {
    using ReturnType = std::future<std::invoke_result_t<F, Args...>>;

    auto task = std::make_unique<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<F>(f), std::forward<Args...>(args...)));

    auto res{task->get_future()};
    // condition variable + mutex
    {
      std::scoped_lock lck{queue_mutex_};
      tasks_.push([tsk = std::move(task)]() { (*tsk)(); });
    }
    cv_.notify_one();
    return res;
  }

private:
  void WorkerThread();
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;
  std::mutex queue_mutex_;
  std::condition_variable cv_;
  size_t num_threads_;
  bool stop_;
};

} // namespace ThreadPool

#endif // THREAD_POOL_H_