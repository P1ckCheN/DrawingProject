// @brief: task queue class realization
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v3.0
// @reversion: last revised by Buckychen on 2021-07-07
#include "taskqueue.h"
#include "threadpool.h"

TaskQueue *TaskQueue::task_queue_singleton = NULL;

TaskQueue *TaskQueue::Instance() {
  if (task_queue_singleton == NULL) {
    task_queue_singleton = new TaskQueue;
  }
  return task_queue_singleton;
}

TaskQueue::TaskQueue() {
  task_queue_mutex = CreateMutex(NULL, FALSE, NULL);
}

TaskQueue::~TaskQueue() {
  while (!task_queue.empty()) {
    delete task_queue.front();
    task_queue.pop();
  }
  CloseHandle(task_queue_mutex);
  task_queue_mutex = NULL;
}

void TaskQueue::Lock() {
  WaitForSingleObject(task_queue_mutex, INFINITE);
}

void TaskQueue::Unlock() {
  ReleaseMutex(task_queue_mutex);
}

// Convert the object to function
void TaskQueue::TaskConvert(void* param) {
  if (param == NULL) {
    return;
  }
  // transform and process
  TaskObjectItem* task_item = static_cast<TaskObjectItem*> (param);
  task_item->task_process->TaskProcess(task_item->task_param);
  delete task_item;
}

void TaskQueue::Push(TaskAbstract* task_process, void* task_param) {
  TaskObjectItem* task_item = new TaskObjectItem(task_process, task_param);
  task_queue.push(new TaskToExcution(TaskConvert, task_item));
}

TaskToExcution* TaskQueue::Front() {
  if (task_queue.empty()) {
    return NULL;
  }
  else {
    return static_cast<TaskToExcution*>(task_queue.front());
  }
}

void TaskQueue::Pop() {
  if (!task_queue.empty()) {
    task_queue.pop();
  }
}

size_t TaskQueue::Size() {
  return task_queue.size();
}

bool TaskQueue::isEmpty() {
  return task_queue.empty();
}