/**
 * \file freepainting.h
 * \brief 主程序头文件
 *
 * 定义结构体变量和全局变量
 *
 * \author 陈瑞佳
 * \version 1.2
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
	TYPE_LINE,													///< 形状：直线	
	TYPE_RECT,													///< 形状：矩形
	TYPE_ELLIPSE,												///< 形状：椭圆
};

enum ColorParm
{
	TYPE_RED,													///< 颜色：红色
	TYPE_BLUE,													///< 颜色：蓝色
	TYPE_GREEN,													///< 颜色：绿色
};

enum LinewidthParm
{
	TYPE_THIN,													///< 粗细度：细
	TYPE_MEDIUM,												///< 粗细度：中
	TYPE_THICK													///< 粗细度：粗
};

struct DrawParm	 
{
	POINT ptBegin;												///< 起始点
	POINT ptEnd;												///< 终止点
	ShapeParm shape_parm;										///< 形状
	std::vector<int> color_parm;								///< 颜色
	int LinewidthParm;											///< 线条宽度
};

// 运行时所需全局变量
static bool flag_clear;											///< 清除画板
static bool flag_readxmlfile;									///< 是否读取文件

static POINT ptShapeBegin;										///< 起始点
static POINT ptShapeEnd;										///< 终止点

static ShapeParm type_shape;									///< 形状类型
static ColorParm type_color;									///< 颜色类型
static LinewidthParm type_linewidth;							///< 线条宽度类型
static std::vector<DrawParm> draw_parm;							///< 保存历史绘制数组

static int color[3];											///< 颜色值
static int linewidth;											///< 线条宽度值

static std::unordered_map<std::string, int*> color_cache;		///< 哈希表，对应颜色映射
static std::unordered_map<std::string, int> linewidth_cache;	///< 哈希表，对应线条宽度映射

// 函数定义
VOID SaveDrawing(HWND hwnd, HDC hdc, POINT ptBegin, POINT ptEnd,
	std::vector<DrawParm>& draw_parm, ShapeParm type_shape, ColorParm type_color, LinewidthParm type_linewidth);

VOID ClearDrawing(std::vector<DrawParm>& draw_parm);

VOID ThreadReadXmlFile(PVOID pvoid);

#endif // !FREEPAINTING_H__