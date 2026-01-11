#include "thread_pool.h"

namespace ThreadPool {

ThreadPool::ThreadPool(size_t numThreads) : num_threads_(numThreads) {
  // Initialize thread pool with numThreads
  workers_.reserve(num_threads_);
  for (size_t i = 0; i < num_threads_; ++i) {
    workers_.emplace_back(&ThreadPool::WorkerThread, this);
  }
}

void ThreadPool::WorkerThread() {
  while (true) {
  }
}

} // namespace ThreadPool
