#include "thread_pool.h"
#include <iostream>

int main() {
  ThreadPool::ThreadPool tp{10, 50};
  std::function<int()> task1 = []() {
    std::cout << "I am task one" << std::endl;
    return 1;
  };
  auto task2 = []() {
    std::cout << "I am task two" << std::endl;
    return 2;
  };
  auto task3 = []() {
    std::cout << "I am task three" << std::endl;
    return 3;
  };
  auto task4 = []() {
    std::cout << "I am task four" << std::endl;
    return 4;
  };

  tp.submit(task1);
  tp.submit(task2);
  tp.submit(task3);
  tp.submit(task4);
  return 0;
}