// @brief: task queue class
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v3.0
// @reversion: last revised by Buckychen on 2021-07-07
#ifndef TASK_QUEUE_H__
#define TASK_QUEUE_H__

#include <windows.h>
#include <queue>

#include "taskabstract.h"

using TaskFunction = void(*)(void*);

// Task unit item
class TaskToExcution {
public:
  TaskFunction task_function;
  void *task_param;
public:
  TaskToExcution(TaskFunction task_function_, void* task_param_ = NULL) {
    task_function = task_function_;
    task_param = task_param_;
  }
};

// Package task object & parameters
class TaskObjectItem {
public:
  // Abstract class pointer
  TaskAbstract* task_process;
  void* task_param;
  TaskObjectItem(TaskAbstract* task_process_, void* task_param_ = NULL) {
    task_process = task_process_;
    task_param = task_param_;
  }
};

class TaskQueue {
public:
  // Task queue singeleton
  static TaskQueue *Instance();
  ~TaskQueue();

  void Lock();
  void Unlock();

  void Push(TaskAbstract* task_process, void* task_param = NULL);
  void Pop();

  size_t Size();
  bool isEmpty();

  TaskToExcution *Front();

private:
  TaskQueue();
  // convert and handle the task object
  static void TaskConvert(void* param);

private:
  static TaskQueue *task_queue_singleton;
  std::queue<TaskToExcution*> task_queue;
  HANDLE task_queue_mutex;

  
};

#endif // !TASK_QUEUE_H__

