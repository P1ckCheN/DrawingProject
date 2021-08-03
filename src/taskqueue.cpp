// @brief: task queue class realization
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v3.0
// @reversion: last revised by Buckychen on 2021-07-07
#include "taskqueue.h"
#include "threadpool.h"

TaskQueue *TaskQueue::task_queue_singleton_ = NULL;

TaskQueue *TaskQueue::Instance() {
  if (task_queue_singleton_ == NULL) {
    task_queue_singleton_ = new TaskQueue;
  }
  return task_queue_singleton_;
}

TaskQueue::TaskQueue() {
  task_queue_mutex_ = CreateMutex(NULL, FALSE, NULL);
}

TaskQueue::~TaskQueue() {
  while (!task_queue_.empty()) {
    delete task_queue_.front();
    task_queue_.pop();
  }
  CloseHandle(task_queue_mutex_);
  task_queue_mutex_ = NULL;
}

void TaskQueue::Lock() {
  WaitForSingleObject(task_queue_mutex_, INFINITE);
}

void TaskQueue::Unlock() {
  ReleaseMutex(task_queue_mutex_);
}

// Convert the object to function
void TaskQueue::TaskConvert(void* param) {
  if (param == NULL) {
    return;
  }
  TaskObjectItem* task_item = static_cast<TaskObjectItem*> (param);
  task_item->task_process_->TaskProcess(task_item->task_param_);
  delete task_item;
}

void TaskQueue::Push(TaskAbstract* task_process, void* task_param) {
  TaskObjectItem* task_item = new TaskObjectItem(task_process, task_param);
  task_queue_.push(new TaskToExcution(TaskConvert, task_item));
}

TaskToExcution* TaskQueue::Front() {
  if (task_queue_.empty()) {
    return NULL;
  }
  else {
    return static_cast<TaskToExcution*>(task_queue_.front());
  }
}

void TaskQueue::Pop() {
  if (!task_queue_.empty()) {
    task_queue_.pop();
  }
}

size_t TaskQueue::Size() {
  return task_queue_.size();
}

bool TaskQueue::isEmpty() {
  return task_queue_.empty();
}