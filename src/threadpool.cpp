// @brief: thread pool class realization
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v3.0
// @reversion: last revised by Buckychen on 2021-07-07
#include "threadpool.h"
#include "taskobject.h"

ThreadPool* ThreadPool::thread_pool_singleton = NULL;

HANDLE ThreadPool::thread_process_semaphore = 0;

// Get singleton of thread pool
ThreadPool* ThreadPool::GetSingleton() {
  if (thread_pool_singleton == NULL) {
    thread_pool_singleton = new ThreadPool;
  }
  return thread_pool_singleton;
}

// Initialize the pool and set suitable size
ThreadPool::ThreadPool() {
  // Get Singleton of task queue
  task_queue_singleton = TaskQueue::Instance();
  // Exclusive of thread pool
  thread_pool_mutex = CreateMutex(NULL, FALSE, NULL);
  // Notify the task to excute
  task_process = CreateSemaphore(NULL, 0, 0x7FFFFFFF, NULL);
  // Delete all the thread
  thread_pool_delete = CreateEvent(NULL, TRUE, FALSE, NULL);
  // Delete all the thread
  thread_pool_complete_delete = CreateEvent(NULL, FALSE, FALSE, NULL);

  assert(task_queue_singleton);
  assert(thread_pool_mutex);
  assert(task_process);
  assert(thread_pool_complete_delete);
 
  // Judge the suitable number of thread
  if (TaskObject::task_type == TaskType::IO_INTENSIVE) {
    // IO intensive type: 2 * cpu_num + 1
    initial_size = 2 * GetProcessorNum() + 1;
  }
  else {
    // CPU intensive type: 1 * cpu_num + 1
    initial_size = GetProcessorNum() + 1;
  }
  CreateThreadPool(initial_size);
}

int ThreadPool::GetProcessorNum()
{
  SYSTEM_INFO system_info;
  GetSystemInfo(&system_info);
  return system_info.dwNumberOfProcessors;
}

// Clear all the task in queue 
ThreadPool::~ThreadPool() {
  while (true) {
    task_queue_singleton->Lock();
    if (task_queue_singleton->Size() == 0) {
      task_queue_singleton->Unlock();
      break;
    }
    else {
      task_queue_singleton->Unlock();
      continue;
    }
  }
  // Delete the threads in thread pool
  SetEvent(thread_pool_delete);
  // Get signal of deleted
  WaitForSingleObject(thread_pool_complete_delete, INFINITE);
  // Colse event handle
  CloseHandle(task_process);
  CloseHandle(thread_pool_delete);
  CloseHandle(thread_pool_complete_delete);
  task_process = NULL;
  thread_pool_delete = NULL;
  thread_pool_complete_delete = NULL;

}

// Create thread pool of given size
void ThreadPool::CreateThreadPool(int thread_num) {
  WaitForSingleObject(thread_pool_mutex, INFINITE);
  ThreadExcution* thread_excution_temp = NULL;
  for (int i = 0; i < thread_num; ++i) {
    // Start thread of given size
    thread_excution_temp = new ThreadExcution(ThreadWorkFunction, (void*)this);
    thread_excution_temp->Start();
    thread_pool.emplace_back(thread_excution_temp);
  }
  ReleaseMutex(thread_pool_mutex);
}

void ThreadPool::ExcuteTask() {
  ReleaseSemaphore(task_process, 1, NULL);
}

// Submit the task object and excute
void ThreadPool::SubmitTaskIntoThreadPool(TaskObject* task_object, void* task_param) {
  task_queue_singleton->Lock();
  task_queue_singleton->Push(task_object, task_param);
  task_queue_singleton->Unlock();
  ExcuteTask();
}

int ThreadPool::GetThreadPoolSize() {
  WaitForSingleObject(thread_pool_mutex, INFINITE);
  int thread_count = thread_pool.size();
  ReleaseMutex(thread_pool_mutex);
  return thread_count;
}

// Threadpool function
// Excute tasks or delete thread pool, depends on the instructions
void ThreadPool::ThreadWorkFunction(void* param) {
  if (param == NULL) {
    return;
  }
  ThreadPool* thread_pool_this = static_cast<ThreadPool*> (param);
  HANDLE instruction[2] = {
    // There are tasks in queue
    thread_pool_this->task_process,
    // Need to delete pool
    thread_pool_this->thread_pool_delete
  };
  bool flag_has_task = false;
  bool flag_thread_pool_delete = false;
  DWORD signal_temp = 0;
  TaskToExcution* task = NULL;
  while (true) {
    signal_temp = WaitForMultipleObjects(2, instruction, false, INFINITE);
    if (signal_temp == WAIT_OBJECT_0) {
      // Take out the task from queue
      thread_pool_this->task_queue_singleton->Lock();
      flag_has_task = !thread_pool_this->task_queue_singleton->isEmpty();
      if (flag_has_task) {
        task = thread_pool_this->task_queue_singleton->Front();
        thread_pool_this->task_queue_singleton->Pop();
      }
      thread_pool_this->task_queue_singleton->Unlock();
      // Excute task
      if (flag_has_task && task != NULL) {
        task->task_function(task->task_param);
        delete task;
        task = NULL;
      }
    }
    else if (signal_temp == WAIT_OBJECT_0 + 1) {
      flag_thread_pool_delete = true;
      break;
    }
  }
  // Delete threads in thread pool, must get the mutex
  WaitForSingleObject(thread_pool_this->thread_pool_mutex, INFINITE);
  if (!thread_pool_this->thread_pool.empty() && flag_thread_pool_delete) {
    for (int i = thread_pool_this->thread_pool.size() - 1; i >= 0; --i) {
      delete static_cast<ThreadExcution*>(thread_pool_this->thread_pool[i]);
      thread_pool_this->thread_pool.pop_back();
    }
    // Delete done
    SetEvent(thread_pool_this->thread_pool_complete_delete);
  }
  ReleaseMutex(thread_pool_this->thread_pool_mutex);
  return;
}