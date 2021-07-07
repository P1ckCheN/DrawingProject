// @brief: thread excution class relization
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v3.0
// @reversion: last revised by Buckychen on 2021-07-07
#include "threadexcution.h"

ThreadExcution::ThreadExcution() {
  thread_id = 0;
  thread_handle = NULL;
}

// Initialize task submit and excute function
ThreadExcution::ThreadExcution(TaskAbstract* task_abstract, void* task_param) {
  flag_task = true;
  task_object_item.task_abstract = task_abstract;
  task_object_item.task_param = task_param;
}

// Initialize function that control thread pool
ThreadExcution::ThreadExcution(TaskFunction task_function, void* task_param) {
  flag_task = false;
  task_function_item.task_function = task_function;
  task_function_item.task_param = task_param;
}

ThreadExcution::~ThreadExcution() {
  if (thread_handle != NULL) {
    CloseHandle(thread_handle);
    thread_handle = NULL;
  }
}

// Start the thread pool
bool ThreadExcution::Start() {
  thread_handle = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, this, 0, &thread_id);
  if (thread_handle == NULL) {
    thread_id = 0;
    return false;
  }
  else {
    return true;
  }
}

unsigned int ThreadExcution::GetThreadId() const {
  return thread_id;
}

HANDLE ThreadExcution::GetThreadHandle() const {
  return thread_handle;
}

void ThreadExcution::Run() {
  // Run task and active threads
  if (flag_task == true) {
    if (task_object_item.task_abstract != NULL) {
      task_object_item.task_abstract->TaskProcess(task_object_item.task_param);
    }
  }
  // Set threads in pool.
  else {
    if (task_function_item.task_function != NULL) {
      task_function_item.task_function(task_function_item.task_param);
    }
  }
  }
  
unsigned int WINAPI ThreadExcution::ThreadFunction(void* param) {
  if (param == NULL) {
    return -1;
  }
  ((ThreadExcution*)(param))->Run();
}


