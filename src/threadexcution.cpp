// @brief: thread excution class relization
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v3.0
// @reversion: last revised by Buckychen on 2021-07-07
#include "threadexcution.h"

ThreadExcution::ThreadExcution() {
  thread_id_ = 0;
  thread_handle_ = NULL;
}

ThreadExcution::ThreadExcution(TaskAbstract* task_abstract, void* task_param) {
  flag_task_ = true;
  task_object_item_.task_abstract = task_abstract;
  task_object_item_.task_param = task_param;
}

ThreadExcution::ThreadExcution(TaskFunction task_function, void* task_param) {
  flag_task_ = false;
  task_function_item_.task_function = task_function;
  task_function_item_.task_param = task_param;
}

ThreadExcution::~ThreadExcution() {
  if (thread_handle_ != NULL) {
    CloseHandle(thread_handle_);
    thread_handle_ = NULL;
  }
}


bool ThreadExcution::Start() {
  thread_handle_ = (HANDLE)_beginthreadex(NULL, 
                                          0, 
                                          ThreadFunction, 
                                          this, 
                                          0, 
                                          &thread_id_);
  if (thread_handle_ == NULL) {
    thread_id_ = 0;
    return false;
  } else {
    return true;
  }
}

unsigned int ThreadExcution::GetThreadId() const {
  return thread_id_;
}

HANDLE ThreadExcution::GetThreadHandle() const {
  return thread_handle_;
}

void ThreadExcution::Run() {
  if (flag_task_ == true) {
    if (task_object_item_.task_abstract != NULL) {
      task_object_item_.task_abstract->
        TaskProcess(task_object_item_.task_param);
    }
  } else {
    if (task_function_item_.task_function != NULL) {
      task_function_item_.task_function(task_function_item_.task_param);
    }
  }
}
  
unsigned int WINAPI ThreadExcution::ThreadFunction(void* param) {
  if (param == NULL) {
    return -1;
  }
  ((ThreadExcution*)(param))->Run();
}


