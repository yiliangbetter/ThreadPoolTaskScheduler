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
  explicit ThreadPool(size_t num_threads, size_t max_queue_size);
  ~ThreadPool();

  template <typename F, typename... Args>
  std::future<std::invoke_result_t<F, Args...>> submit(F &&f, Args &&...args) {
    using ReturnType = std::future<std::invoke_result_t<F, Args...>>;

    auto task = std::make_shared<
        std::packaged_task<std::invoke_result_t<F, Args...>()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    ReturnType res = task->get_future();
    // condition variable + mutex
    {
      std::unique_lock<std::mutex> lck{queue_mutex_};
      queue_full_.wait(lck, [&]() { return tasks_.size() <= max_queue_size_; });
      tasks_.push([task]() { (*task)(); });
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
  std::condition_variable queue_full_;
  size_t num_threads_;
  size_t max_queue_size_;
  bool stop_;
};

} // namespace ThreadPool

#endif // THREAD_POOL_H_