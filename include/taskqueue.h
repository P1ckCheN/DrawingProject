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
  TaskFunction task_function_;
  void *task_param_;
public:
  TaskToExcution(TaskFunction task_function, void* task_param = NULL) {
    task_function_ = task_function;
    task_param_ = task_param;
  }
};

class TaskObjectItem {
public:
  TaskAbstract* task_process_;
  void* task_param_;
  TaskObjectItem(TaskAbstract* task_process, void* task_param = NULL) {
    task_process_ = task_process;
    task_param_ = task_param;
  }
};

class TaskQueue {
public:
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
  static void TaskConvert(void* param);

  static TaskQueue *task_queue_singleton_;
  std::queue<TaskToExcution*> task_queue_;
  HANDLE task_queue_mutex_;
};

#endif // !TASK_QUEUE_H__

