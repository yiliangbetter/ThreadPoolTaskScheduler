#include <cstddef>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class ThreadPool {
public:
  ThreadPool(size_t numThreads);
  ~ThreadPool();

  void enqueue(std::function<void()> task);

private:
  std::vector<std::thread> workers;
  std::vector<std::function<void()>> tasks;
};