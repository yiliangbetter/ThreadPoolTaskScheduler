#include "thread_pool.h"
#include <utility>

namespace ThreadPool {

ThreadPool::ThreadPool(size_t num_threads, size_t max_queue_size)
    : num_threads_(num_threads), max_queue_size_{max_queue_size}, stop_(false) {
  // Initialize thread pool with numThreads
  workers_.reserve(num_threads_);
  for (size_t i = 0; i < num_threads_; ++i) {
    workers_.emplace_back(&ThreadPool::WorkerThread, this);
  }
}

ThreadPool::~ThreadPool() {
  stop_ = true;
  cv_.notify_all();

  for (auto &worker : workers_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

void ThreadPool::WorkerThread() {
  while (true) {
    std::function<void()> task;
    {
      std::unique_lock lck{queue_mutex_};
      cv_.wait(lck, [&]() { return !tasks_.empty() || stop_; });

      if (stop_ && tasks_.empty()) {
        return;
      }

      task = std::move(tasks_.front());
      tasks_.pop();
      queue_full_.notify_one();
    }
    task();
  }
}

} // namespace ThreadPool
