// @brief: thread pool class realization
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v3.0
// @reversion: last revised by Buckychen on 2021-07-07
#include "threadpool.h"
#include "taskobject.h"

ThreadPool* ThreadPool::thread_pool_singleton_ = NULL;
HANDLE ThreadPool::thread_process_semaphore_ = 0;

ThreadPool* ThreadPool::GetSingleton() {
  if (thread_pool_singleton_ == NULL) {
    thread_pool_singleton_ = new ThreadPool;
  }
  return thread_pool_singleton_;
}

ThreadPool::ThreadPool() {
  task_queue_singleton_ = TaskQueue::Instance();
  thread_pool_mutex_ = CreateMutex(NULL, FALSE, NULL);
  task_process_ = CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
  thread_pool_delete_ = CreateEvent(NULL, TRUE, FALSE, NULL);
  thread_pool_complete_delete_ = CreateEvent(NULL, FALSE, FALSE, NULL);
  assert(task_queue_singleton_);
  assert(thread_pool_mutex_);
  assert(task_process_);
  assert(thread_pool_complete_delete_);
 
  if (TaskObject::task_type_ == TaskType::IO_INTENSIVE) {
    initial_size_ = 2 * GetProcessorNum() + 1;
  } else {
    initial_size_ = GetProcessorNum() + 1;
  }
  CreateThreadPool(initial_size_);
}

int ThreadPool::GetProcessorNum() {
  SYSTEM_INFO system_info;
  GetSystemInfo(&system_info);
  return system_info.dwNumberOfProcessors;
}

ThreadPool::~ThreadPool() {
  while (true) {
    task_queue_singleton_->Lock();
    if (task_queue_singleton_->Size() == 0) {
      task_queue_singleton_->Unlock();
      break;
    } else {
      task_queue_singleton_->Unlock();
      continue;
    }
  }
  // Delete the threads in thread pool
  SetEvent(thread_pool_delete_);
  WaitForSingleObject(thread_pool_complete_delete_, INFINITE);
  CloseHandle(task_process_);
  CloseHandle(thread_pool_delete_);
  CloseHandle(thread_pool_complete_delete_);
  task_process_ = NULL;
  thread_pool_delete_ = NULL;
  thread_pool_complete_delete_ = NULL;
}

void ThreadPool::CreateThreadPool(int thread_num) {
  WaitForSingleObject(thread_pool_mutex_, INFINITE);
  ThreadExcution* thread_excution_temp = NULL;
  for (int i = 0; i < thread_num; ++i) {
    // Start thread of given size
    thread_excution_temp = new ThreadExcution(ThreadWorkFunction, (void*)this);
    thread_excution_temp->Start();
    thread_pool_.emplace_back(thread_excution_temp);
  }
  ReleaseMutex(thread_pool_mutex_);
}

void ThreadPool::ExcuteTask() {
  ReleaseSemaphore(task_process_, 1, NULL);
}

void ThreadPool::SubmitTaskIntoThreadPool(TaskObject* task_object, 
                                          void* task_param) {
  task_queue_singleton_->Lock();
  task_queue_singleton_->Push(task_object, task_param);
  task_queue_singleton_->Unlock();
  ExcuteTask();
}

int ThreadPool::GetThreadPoolSize() {
  WaitForSingleObject(thread_pool_mutex_, INFINITE);
  int thread_count = thread_pool_.size();
  ReleaseMutex(thread_pool_mutex_);
  return thread_count;
}

void ThreadPool::ThreadWorkFunction(void* param) {
  if (param == NULL) {
    return;
  }
  ThreadPool* thread_pool_this = static_cast<ThreadPool*> (param);
  HANDLE instruction[2] = {
    thread_pool_this->task_process_,
    thread_pool_this->thread_pool_delete_
  };
  bool flag_has_task = false;
  bool flag_thread_pool_delete = false;
  DWORD signal_temp = 0;
  TaskToExcution* task = NULL;
  while (true) {
    signal_temp = WaitForMultipleObjects(2, instruction, false, INFINITE);
    if (signal_temp == WAIT_OBJECT_0) {
      // Take out the task from queue
      thread_pool_this->task_queue_singleton_->Lock();
      flag_has_task = !thread_pool_this->task_queue_singleton_->isEmpty();
      if (flag_has_task) {
        task = thread_pool_this->task_queue_singleton_->Front();
        thread_pool_this->task_queue_singleton_->Pop();
      }
      thread_pool_this->task_queue_singleton_->Unlock();
      if (flag_has_task && task != NULL) {
        task->task_function_(task->task_param_);
        delete task;
        task = NULL;
      }
    } else if (signal_temp == WAIT_OBJECT_0 + 1) {
      flag_thread_pool_delete = true;
      break;
    }
  }
  // Delete threads in thread pool
  WaitForSingleObject(thread_pool_this->thread_pool_mutex_, INFINITE);
  if (!thread_pool_this->thread_pool_.empty() && flag_thread_pool_delete) {
    for (int i = thread_pool_this->thread_pool_.size() - 1; i >= 0; --i) {
      delete static_cast<ThreadExcution*>(thread_pool_this->thread_pool_[i]);
      thread_pool_this->thread_pool_.pop_back();
    }
    SetEvent(thread_pool_this->thread_pool_complete_delete_);
  }
  ReleaseMutex(thread_pool_this->thread_pool_mutex_);
  return;
}