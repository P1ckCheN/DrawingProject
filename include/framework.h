/**
 * \file framework.h
 * \brief 标准系统包含文件的包含文件，或特定于项目的包含文件
 *
 * 主程序
 *
 * \author 陈瑞佳
 * \version 1.0
 * \date 2021/06/17
 */

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
#include "targetver.h"
#include "parsingxml.h"
#include "errorhandle.h"
#include "freepainting.h"
#include "paintingstruct.h"
#include "tinyxml/tinyxml.h"

#endif