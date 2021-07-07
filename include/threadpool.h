// @brief: thread pool class
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v3.0
// @reversion: last revised by Buckychen on 2021-07-07
#ifndef THREADPOLL_H__
#define THREADPOLL_H__

#include <windows.h>
#include <process.h>
#include <queue>

#include "taskqueue.h"
#include "threadexcution.h"

// Simulate the thread pool function
// Create thread pool of given size, it will be a container of threads
// Example:
class ThreadPool {
public:
  TaskQueue* task_queue_singleton;
  // Thread handle of processing threads in pool
  static HANDLE thread_process_semaphore;
  // Process thread
  HANDLE task_process;
  // Thread pool mutex
  HANDLE thread_pool_mutex;
  // Clear event
  HANDLE thread_pool_delete;
  // Clear done event
  HANDLE thread_pool_complete_delete;
  // Singleton 
  static ThreadPool* thread_pool_singleton;
  // Thread pool of vector
  std::vector<ThreadExcution* > thread_pool;

public:
  ThreadPool();
  ~ThreadPool();
  void ExcuteTask();
  int GetThreadPoolSize();
  void CreateThreadPool(int thread_num);
  void SubmitTaskIntoThreadPool(TaskObject* task_object, void* task_param);

  static ThreadPool* GetSingleton();  
  static void ThreadWorkFunction(void* param);

private:
  int initial_size;
  int GetProcessorNum();
};

#endif // !THREADPOLL_H__
