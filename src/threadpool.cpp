// @brief: thread pool function realization
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v2.6
// @reversion: last revised by Buckychen on 2021-07-05
#include "threadpool.h"

ThreadPool* ThreadPool::thread_pool = nullptr;

ThreadPool::ThreadPool() {
  thread_process = 0;
}

// Create threads of size given, plus one process thread.
ThreadPool::ThreadPool(int thread_max_num) {
  for (int i = 0; i < thread_max_num; ++i) {
    HANDLE thread_handle_temp;
    thread_handle_temp = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, (void*)i, CREATE_SUSPENDED, NULL);
    ThreadInfo thread_info_temp;
    thread_info_temp.thread_handle = thread_handle_temp;
    thread_info_temp.thread_id = i;
    thread_info_temp.thread_status = ThreadStatus::FREE_STATE;
    // Store thread infomation into hash map
    thread_info_map[i] = thread_info_temp;
  }
  thread_process = (HANDLE)_beginthreadex(NULL, 0, CheckExecutionMsgThread, NULL, 0, NULL);
}

ThreadPool::~ThreadPool() {
  ClearThreadPool();
}

unsigned int WINAPI ThreadPool::ThreadFunction(void* param) {
  while (true) {
    ThreadPool::GetThreadPool()->RunThread((int)param);
    ThreadPool::GetThreadPool()->ResetThread((int)param);
  }
  return 0;
}

// Check if there is any messages in the queue
unsigned int WINAPI ThreadPool::CheckExecutionMsgThread(void* param) {
  while (true) {
    ThreadPool::GetThreadPool()->AllocateTask();
    Sleep(50);
  }
  return 0;
}

// Push task into message queue
void ThreadPool::PushMsgToDealQueue(ThreadCallBack excution_function, void* excution_param) {
  ThreadExcution thread_excution;
  thread_excution.excution_function = excution_function;
  thread_excution.excution_param = excution_param;
  thread_excution_queue.push(thread_excution);
}

// If there is any free threads to run task
void ThreadPool::AllocateTask() {
  if (thread_excution_queue.size() != 0) {
    ThreadExcution thread_excution = thread_excution_queue.front();
    for (thread_iter = thread_info_map.begin(); thread_iter != thread_info_map.end(); ++thread_iter) {
      if ((thread_iter->second).thread_status == ThreadStatus::FREE_STATE) {
        thread_iter->second.thread_function = thread_excution.excution_function;
        thread_iter->second.thread_param = thread_excution.excution_param;
        thread_iter->second.thread_status = ThreadStatus::RUNNING_STATE;
        // Activate free thread
        ResumeThread(thread_iter->second.thread_handle);
        break;
      }
    }
  }
  return;
}

// Change the thread status and run task
void ThreadPool::RunThread(int thread_id) {
  if (thread_info_map[thread_id].thread_status == ThreadStatus::RUNNING_STATE) {
    // Pass the parameter and run task
    thread_info_map[thread_id].thread_function(thread_info_map[thread_id].thread_param);
  }
}

// If the task have excuted change the thread status and suspend the thread
void ThreadPool::ResetThread(int thread_id) {
  thread_info_map[thread_id].thread_status = ThreadStatus::FREE_STATE;
  SuspendThread(thread_info_map[thread_id].thread_handle);
}

// Close the thread pool
void ThreadPool::ClearThreadPool() {
  CloseHandle(thread_process);
  for (thread_iter = thread_info_map.begin(); thread_iter != thread_info_map.end(); ++thread_iter) {
    CloseHandle(thread_iter->second.thread_handle);
  }
}
