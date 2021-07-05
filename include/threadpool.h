// @brief: thread pool struct
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v2.6
// @reversion: last revised by Buckychen on 2021-07-05
#ifndef THREADPOLL_H__
#define THREADPOLL_H__

#include <windows.h>
#include <process.h>

#include <queue>
#include <list>
#include <unordered_map>

// Declare the form of callback function

typedef void(*ThreadCallBack) (void* param);

enum class ThreadStatus {
  RUNNING_STATE = 0,
  FREE_STATE    = 1,
};

struct ThreadInfo {
  int            thread_id;
  void*          thread_param;
  HANDLE         thread_handle;
  ThreadStatus   thread_status;
  ThreadCallBack thread_function;
};

struct ThreadExcution {
  void*          excution_param;
  ThreadCallBack excution_function;
};


// Simulate the thread pool function
// Create thread pool of given size, it will be a container of threads
// Example:
//  int thread_pool_size = 3;
//  ThreadPool::CreatThreadPool(thread_pool_size);
//  ThreadPool::PostTaskPool(TaskReadXmlFile);

class ThreadPool {
public:
  std::queue<ThreadExcution>                    thread_excution_queue;
  std::unordered_map<int, ThreadInfo>           thread_info_map;
  std::unordered_map<int, ThreadInfo>::iterator thread_iter;
  // Thread handle of processing threads in pool
  HANDLE thread_process;

public:
  // Singleton 
  static ThreadPool* thread_pool;
  static ThreadPool* CreateThreadPool(int thread_max_num) {
    if (thread_pool == nullptr) {
      thread_pool = new ThreadPool(thread_max_num);
    }
    return thread_pool;
  }
  static ThreadPool* GetThreadPool() {
    return thread_pool;
  }
  static void SubmitTaskIntoThreadPool(ThreadCallBack task, void* param) {
    GetThreadPool()->PushMsgToDealQueue(task, param);
  }
  static void CleanThreadPool() {
    GetThreadPool()->ClearThreadPool();
  }
  ThreadPool();
  ThreadPool(int thread_max_num);
  ~ThreadPool();
  void PushMsgToDealQueue(ThreadCallBack excution_function, void* excution_param);
  void AllocateTask();
  void RunThread(int thread_id);
  void ResetThread(int thread_id);
  void ClearThreadPool();
  static unsigned int WINAPI ThreadFunction(void* param);
  static unsigned int WINAPI CheckExecutionMsgThread(void* param);
};

#endif // !THREADPOLL_H__
