#include "thread_pool.h"
#include <iostream>
#include <unistd.h>

std::mutex cout_mutex;

int main() {
  ThreadPool::ThreadPool tp{2, 4};
  std::function<int()> task1 = []() {
    {
      std::scoped_lock lck{cout_mutex};
      std::cout << "processing task 1" << std::endl;
    }
    sleep(1);
    return 1;
  };
  auto task2 = []() {
    {
      std::scoped_lock lck{cout_mutex};
      std::cout << "processing task 2" << std::endl;
    }
    sleep(1);
    return 2;
  };
  auto task3 = []() {
    {
      std::scoped_lock lck{cout_mutex};
      std::cout << "processing task 3" << std::endl;
    }
    sleep(1);
    return 3;
  };
  auto task4 = []() {
    {
      std::scoped_lock lck{cout_mutex};
      std::cout << "processing task 4" << std::endl;
    }
    sleep(1);
    return 4;
  };

  std::cout << "submitting task 1" << std::endl;
  auto fut1 = tp.submit(task1);
  std::cout << "submitting task 2" << std::endl;
  auto fut2 = tp.submit(task2);
  std::cout << "submitting task 3" << std::endl;
  auto fut3 = tp.submit(task3);
  std::cout << "submitting task 4" << std::endl;
  auto fut4 = tp.submit(task4);

  // Wait for all tasks to complete
  std::cout << "Task 1 result: " << fut1.get() << std::endl;
  std::cout << "Task 2 result: " << fut2.get() << std::endl;
  std::cout << "Task 3 result: " << fut3.get() << std::endl;
  std::cout << "Task 4 result: " << fut4.get() << std::endl;

  return 0;
}