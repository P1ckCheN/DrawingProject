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


#include "framework.h"
#include "Resource.h"
#include "parsingxml.h"

// 定义参数结构体
enum ShapeParm
{
	TYPE_LINE,						///< 形状：直线	
	TYPE_RECT,						///< 形状：矩形
	TYPE_ELLIPSE,					///< 形状：椭圆
};

enum ColorParm
{
	TYPE_RED,						///< 颜色：红色
	TYPE_BLUE,						///< 颜色：蓝色
	TYPE_GREEN,						///< 颜色：绿色
};

enum LinewidthParm
{
	TYPE_THIN,						///< 粗细度：细
	TYPE_MEDIUM,					///< 粗细度：中
	TYPE_THICK						///< 粗细度：粗
};

struct DrawParm
{
	POINT ptBegin;
	POINT ptEnd;
	ShapeParm shape_parm;
	//ColorParm color_parm;
	//LinewidthParm LinewidthParm;
	std::vector<int> color_parm;
	int LinewidthParm;
};
void SaveDrawing(HWND hwnd, HDC hdc, POINT ptBegin, POINT ptEnd,
	std::vector<DrawParm>& draw_parm, ShapeParm type_shape, ColorParm type_color, LinewidthParm type_linewidth);
#endif