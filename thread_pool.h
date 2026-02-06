#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

namespace ThreadPool {

enum class Priority: int8_t{
  high = 0,
  medium = 1,
  low = 2,
}

struct Task
{
  Priority priority;
  std::function<int()> callable;
};

class ThreadPool {
public:
  explicit ThreadPool(size_t num_threads, size_t max_queue_size);
  ~ThreadPool();

  std::future<int> submit(Task &task) {
    std::future<int> res = task->get_future();
    // condition variable + mutex
    {
      std::unique_lock<std::mutex> lck{queue_mutex_};
      queue_full_.wait(lck, [&]() { return tasks_.size() < max_queue_size_; });
      tasks_.push([task = std::move(task)]() { task.callable(); });
    }
    cv_.notify_one();
    return res;
  }

private:
  void WorkerThread();
  std::vector<std::thread> workers_;
  std::priority_queue<Task, std::vector<Task>, std::function<bool(Task, Task)>>
      tasks_;
  std::mutex queue_mutex_;
  std::condition_variable cv_;
  std::condition_variable queue_full_;
  size_t num_threads_;
  size_t max_queue_size_;
  bool stop_;
};

} // namespace ThreadPool

#endif // THREAD_POOL_H_