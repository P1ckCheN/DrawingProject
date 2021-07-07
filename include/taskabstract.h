// @brief: Abstract class, use it as interface 
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v3.0
// @reversion: last revised by Buckychen on 2021-07-07
#ifndef TASK_ABSTRACT_H__
#define TASK_ABSTRACT_H__

class TaskAbstract {
public:
  virtual ~TaskAbstract() {}
  // Task processing interface
  virtual void TaskProcess(void* task_param) = 0;
};

#endif // !TASK_ABSTRACT_H__

