// @brief: include files
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v2.3
// @reversion: last revised by Buckychen on 2021-06-30

#ifndef FRAMEWORK_H__
#define FRAMEWORK_H__



// 从 Windows 头文件中排除极少使用的内容
#define WIN32_LEAN_AND_MEAN             

// Windows 头文件
#include <windows.h>

// C 运行时头文件
#include <tchar.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <process.h>

// C++ 运行时头文件
#include <mutex>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <iostream>
#include <unordered_map>

// 自定义头文件
#include "Resource.h"
//#include "targetver.h"
#include "parsingxml.h"
#include "errorhandle.h"
#include "freepainting.h"
#include "paintingstruct.h"
#include "tinyxml/tinyxml.h"

#endif