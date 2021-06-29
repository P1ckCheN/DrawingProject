/**
 * \file freepainting.h
 * \brief 主程序头文件
 *
 * 定义结构体变量和全局变量
 *
 * \author 陈瑞佳
 * \version 2.2
 * \date 2021/06/24
 */

#ifndef FREEPAINTING_H__
#define FREEPAINTING_H__

#include "framework.h"			// 相关头文件

#include <windows.h>			// C库
#include <process.h>

#include <string>				// C++库
#include <vector>
#include <atomic>
#include <unordered_map>

#include "tinyxml/tinyxml.h"	// 其他库的.h

#include "Resource.h"			// 本项目内的.h
#include "parsingxml.h"
#include "errorhandle.h"
#include "freepainting.h"
#include "paintingstruct.h"
	
// 函数声明
unsigned int WINAPI ThreadReadXmlFile(void* p_parm);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#endif // !FREEPAINTING_H__