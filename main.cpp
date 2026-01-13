#include "thread_pool.h"
#include <iostream>

int main() {
  ThreadPool::ThreadPool tp{10};
  auto task1 = []() { std::cout << "I am task one" << std::endl; };
  auto task2 = []() { std::cout << "I am task two" << std::endl; };
  auto task3 = []() { std::cout << "I am task three" << std::endl; };
  auto task4 = []() { std::cout << "I am task four" << std::endl; };

  tp.submit(task1);
  tp.submit(task2);
  tp.submit(task3);
  tp.submit(task4);
  return 0;
}