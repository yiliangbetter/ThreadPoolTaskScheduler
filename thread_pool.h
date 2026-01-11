#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

namespace ThreadPool {

class ThreadPool {
public:
  explicit ThreadPool(size_t numThreads);
  ~ThreadPool();

  void submit(std::function<void()> task);

private:
  void WorkerThread();
  std::vector<std::thread> workers_;
  std::vector<std::function<void()>> tasks_;
  std::mutex queue_mutex_;
  std::condition_variable cv_;
  size_t num_threads_;
};

} // namespace ThreadPool

#endif // THREAD_POOL_H_