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

#include "targetver.h"

// 从 Windows 头文件中排除极少使用的内容
#define WIN32_LEAN_AND_MEAN             

// Windows 头文件
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <iostream>

#endif