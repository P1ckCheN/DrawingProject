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


class ThreadPool {
 public:
  ThreadPool();
  ~ThreadPool();
  void ExcuteTask();
  int GetThreadPoolSize();
  void CreateThreadPool(int thread_num);
  void SubmitTaskIntoThreadPool(TaskObject* task_object, void* task_param);
  static ThreadPool* GetSingleton();  
  static void ThreadWorkFunction(void* param);

  TaskQueue* task_queue_singleton_;
  HANDLE task_process_;
  HANDLE thread_pool_mutex_;
  HANDLE thread_pool_delete_;
  HANDLE thread_pool_complete_delete_;
  std::vector<ThreadExcution* > thread_pool_;
  static ThreadPool* thread_pool_singleton_;
  static HANDLE thread_process_semaphore_;

 private:
  int initial_size_;
  int GetProcessorNum();
};

#endif // !THREADPOLL_H__
