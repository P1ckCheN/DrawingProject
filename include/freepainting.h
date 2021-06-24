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

#include "framework.h"
#include "Resource.h"

 // 画图参数
enum class ShapeParm
{	
	TYPE_LINE,													///< 形状：直线	
	TYPE_RECT,													///< 形状：矩形
	TYPE_ELLIPSE												///< 形状：椭圆
};

enum class ColorParm
{
	TYPE_RED,													///< 颜色：红色
	TYPE_BLUE,													///< 颜色：蓝色
	TYPE_GREEN													///< 颜色：绿色
};

enum class LinewidthParm
{
	TYPE_THIN,													///< 粗细度：细
	TYPE_MEDIUM,												///< 粗细度：中
	TYPE_THICK													///< 粗细度：粗
};

// 存储结构体
struct DrawingParm	 
{
	POINT point_begin;											///< 起始点
	POINT point_end;											///< 终止点
	ShapeParm shape_parm;										///< 形状
	std::vector<int> color_parm;								///< 颜色
	int linewidth_parm;											///< 线条宽度
};

enum class DataState
{
	NO_UPDATE = 0,												///< 未更新
	UPDATEED = 1,												///< 已更新
	DELETED = 2													///< 已删除
};

// 模拟存储类
class Cache
{
public:
	// 成员变量
	DataState state;											///< 当前状态
	std::unordered_map<std::string, int*> color_cache;			///< 颜色存储
	std::unordered_map<std::string, int > linewidth_cache;		///< 线条宽度存储

	// 成员函数
	Cache();													///< 构造函数
	void deletecache();											///< 删除缓存
	Cache& operator=(const Cache& cache);						///< 重载赋值运算符
};



// 画板类
class DrawingBoard
{
private:
	// 成员变量
	bool flag_clear;										    ///< 清除画板
	bool flag_read_xml_file;									///< 是否已经读取文件

	int color[3];												///< 画板对象颜色
	int linewidth;												///< 画板对象线条宽度

	POINT point_begin;											///< 画板对象鼠标起始点
	POINT point_end;											///< 画板对象鼠标终止点

	ShapeParm type_shape;										///< 线条类型
	ColorParm type_color;										///< 颜色类型
	LinewidthParm type_linewidth;								///< 线条宽度类型
	std::vector<DrawingParm> drawing_parm;						///< 参数存储结构体

public:
	// 成员函数
	DrawingBoard() :	
		flag_clear(false), flag_read_xml_file(false),
		color{ 255,0,0 }, linewidth(0),
		point_begin{ 0, 0 }, point_end{ 0, 0 },
		type_shape(ShapeParm::TYPE_LINE), type_color(ColorParm::TYPE_RED), type_linewidth(LinewidthParm::TYPE_THIN) {}
	~DrawingBoard();
	void Drawing(HWND hwnd, HDC hdc);
	void ClearDrawing();
	inline void SetType(ShapeParm shape) { type_shape = shape; }
	inline void SetColor(ColorParm color) { type_color = color; }
	inline void SetLinewidth(LinewidthParm linewidth) { type_linewidth = linewidth; }
	inline void SetRead(bool flag) { flag_read_xml_file = flag; }
	inline bool GetClear() { return flag_clear; }
	inline void SetClear() { flag_clear = true; }
	inline void SetCurrPoint(POINT point_curr_begin, POINT point_curr_end)
	{
		point_begin = point_curr_begin;
		point_end = point_curr_end;
	}
};

// 全局变量
static DrawingBoard board;											///< 画板对象
static POINT point_curr_begin;										///< 起始点
static POINT point_curr_end;										///< 终止点
static Cache color_linewidth_cache;									///< 缓存
static Cache color_linewidth_database;								///< 数据库
	
// 函数声明
void ThreadReadXmlFile(DrawingBoard& board);

void ReadXmlFile1(DrawingBoard& board);

void Transform(int* color_value, std::string& color_value_string);

int ParsingDataCache(ColorParm in_color, LinewidthParm in_linewidth, int* out_color, int& out_linewidth,
	std::unordered_map<std::string, int*>& color_cache, std::unordered_map<std::string, int>& linewidth_cache);

int ParsingDataAtomic(ColorParm in_color, LinewidthParm in_linewidth, int* out_color, int& out_linewidth,
	std::unordered_map<std::string, std::atomic_int*>& color_cache, std::unordered_map<std::string, std::atomic_int>& linewidth_cache);

int ReadXmlFile(DrawingBoard& board);

#endif // !FREEPAINTING_H__