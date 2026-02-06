#include "thread_pool.h"
#include <iostream>
#include <unistd.h>

std::mutex cout_mutex;

int main() {
  ThreadPool::ThreadPool tp{2, 1};
  ThreadPool::Task task1{ThreadPool::Priority::low, []() {
                           {
                             std::scoped_lock lck{cout_mutex};
                             std::cout << "processing task 1" << std::endl;
                           }
                           sleep(5);
                           std::cout << "done task 1" << std::endl;

                           return 1;
                         }};

  ThreadPool::Task task2{ThreadPool::Priority::high, []() {
                           {
                             std::scoped_lock lck{cout_mutex};
                             std::cout << "processing task 2" << std::endl;
                           }
                           sleep(5);
                           std::cout << "done task 2" << std::endl;
                           return 2;
                         }};
  ThreadPool::Task task3{ThreadPool::Priority::medium, []() {
                           {
                             std::scoped_lock lck{cout_mutex};
                             std::cout << "processing task 3" << std::endl;
                           }
                           sleep(5);
                           std::cout << "done task 3" << std::endl;
                           return 3;
                         }};
  ThreadPool::Task task4 = {ThreadPool::Priority::medium, []() {
                              {
                                std::scoped_lock lck{cout_mutex};
                                std::cout << "processing task 4" << std::endl;
                              }
                              sleep(5);
                              std::cout << "done task 4" << std::endl;
                              return 4;
                            }};

  std::cout << "submitting task 1" << std::endl;
  auto fut1 = tp.submit(task1);
  std::cout << "submited task 1" << std::endl;

  std::cout << "submitting task 2" << std::endl;
  auto fut2 = tp.submit(task2);
  std::cout << "submited task 2" << std::endl;
  std::cout << "submitting task 3" << std::endl;
  auto fut3 = tp.submit(task3);
  std::cout << "submited task 3" << std::endl;
  std::cout << "submitting task 4" << std::endl;
  auto fut4 = tp.submit(task4);
  std::cout << "submitted task 4" << std::endl;

  // Wait for all tasks to complete
  std::cout << "Task 1 result: " << fut1.get() << std::endl;
  std::cout << "Task 2 result: " << fut2.get() << std::endl;
  std::cout << "Task 3 result: " << fut3.get() << std::endl;
  std::cout << "Task 4 result: " << fut4.get() << std::endl;

  return 0;
}