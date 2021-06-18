/**
 * \file freepainting.h
 * \brief 主程序头文件
 *
 * 定义结构体
 *
 * \author 陈瑞佳
 * \version 1.0
 * \date 2021/06/17
 */

#ifndef FREEPAINTING_H__
#define FREEPAINTING_H__

#include "Resource.h"
#include "parsingxml.h"

// 定义参数结构体
enum Parm
{
	TYPE_LINE,						///< 形状：直线	
	TYPE_RECT,						///< 形状：矩形
	TYPE_ELLIPSE,					///< 形状：椭圆
	TYPE_RED,						///< 颜色：红色
	TYPE_BLUE,						///< 颜色：蓝色
	TYPE_GREEN,						///< 颜色：绿色
	TYPE_THIN,						///< 粗细度：细
	TYPE_MEDIUM,					///< 粗细度：中
	TYPE_THICK						///< 粗细度：粗
};

#endif