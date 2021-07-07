// @brief: function object inherit from abstract class
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v3.0
// @reversion: last revised by Buckychen on 2021-07-07
#ifndef TASK_OBJECT_H__
#define TASK_OBJECT_H__

#include <windows.h>
#include "errorhandle.h"
#include "parsingxml.h"
#include "paintingstruct.h"
#include "taskabstract.h"
enum class TaskType{
  CPU_INTENSIVE,
  IO_INTENSIVE
};

class TaskObject : public TaskAbstract {
public:
  static const TaskType task_type = TaskType::IO_INTENSIVE;
  static HANDLE read_and_exit_handle[2];
public:
  TaskObject() {
    read_and_exit_handle[0] = CreateSemaphore(NULL, 0, 1, NULL);
    read_and_exit_handle[1] = CreateWaitableTimer(NULL, FALSE, NULL);
    LARGE_INTEGER timer_unit_per_second;
    timer_unit_per_second.QuadPart = 10000000;
    // Call timer after one second
    SetWaitableTimer(read_and_exit_handle[1], &timer_unit_per_second, 10 * 1000, NULL, NULL, FALSE);
  }
  ~TaskObject() {
    ReleaseSemaphore(read_and_exit_handle[0], 1, NULL);
    CloseHandle(read_and_exit_handle[0]);
    CloseHandle(read_and_exit_handle[1]);
  }
  void TaskProcess(void* task_param) {
    while (true) {
      DWORD error_code;
      int read_and_exit_event = WaitForMultipleObjects(2, read_and_exit_handle, FALSE, INFINITE);
      error_code = GetLastError();
      if (read_and_exit_event == WAIT_OBJECT_0) {
        break;
      }
      else if (read_and_exit_event == WAIT_OBJECT_0 + 1) {
        Error::ErrorShow(ReadXmlFile((DrawingBoard*)task_param));
      }
      else if (read_and_exit_event == WAIT_FAILED) {
        Error::ErrorShow(ERROR_HANDLE_WAITOBJECT);
        break;
      }
    }
    return;
  }
};


//class Task {
//public:
//  static HANDLE read_and_exit_handle[2];
//public:
//  static void TaskReadXmlFile(void* param) {
//    while (true) {
//      DWORD error_code;
//      int read_and_exit_event = WaitForMultipleObjects(2, read_and_exit_handle, FALSE, INFINITE);
//      error_code = GetLastError();
//      if (read_and_exit_event == WAIT_OBJECT_0) {
//        break;
//      }
//      else if (read_and_exit_event == WAIT_OBJECT_0 + 1) {
//        Error::ErrorShow(ReadXmlFile((DrawingBoard*)param));
//      }
//      else if (read_and_exit_event == WAIT_FAILED) {
//        Error::ErrorShow(ERROR_HANDLE_WAITOBJECT);
//        break;
//      }
//    }
//    return;
//  }
//};

#endif
