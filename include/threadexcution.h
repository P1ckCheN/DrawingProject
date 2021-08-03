// @brief: thread excution class
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v3.0
// @reversion: last revised by Buckychen on 2021-07-07
#ifndef THREAD_EXCUTION_H__
#define THREAD_EXCUTION_H__

#include <windows.h>
#include <process.h>
#include "taskobject.h"
#include "taskqueue.h"

using TaskFunction = void(*)(void*);

class ThreadExcution {
 public:
  ThreadExcution();
  explicit ThreadExcution(TaskAbstract* task_process, void* task_param = NULL);
  explicit ThreadExcution(TaskFunction task_function, void* task_param = NULL);
  ~ThreadExcution();
  void Run();
  bool Start();
  unsigned int GetThreadId() const;
  HANDLE GetThreadHandle() const;
  static unsigned int WINAPI ThreadFunction(void* param);

 private:
  struct TaskObjectItem {
    TaskAbstract* task_abstract;
    void* task_param;
  } task_object_item_;
  struct TaskFunctionItem {
    TaskFunction task_function;
    void* task_param;
  } task_function_item_;

  unsigned int thread_id_;
  HANDLE thread_handle_;
  bool flag_task_;
};

#endif // !THREAD_EXCUTION_H__
