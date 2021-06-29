#include "paintingstruct.h"

// DrawingBoard类
/**
 * @brief 构造函数
 */
DrawingBoard::DrawingBoard():
	flag_clear(false), flag_read_xml_file(false),
	color{ 0,0,0 }, linewidth(0),
	point_begin{ 0, 0 }, point_end{ 0, 0 },
	type_shape(ShapeParm::TYPE_LINE), 
	type_color(ColorParm::TYPE_RED), 
	type_linewidth(LinewidthParm::TYPE_THIN) 
{

}

/**
 * @brief 析构函数
 */
DrawingBoard::~DrawingBoard()
{
	drawing_parm.clear();
}

/**
 * @brief 清除屏幕函数
 * 右键清屏实现
 */
void DrawingBoard::ClearDrawing()
{
	flag_clear = false;
	drawing_parm.clear();
	return;
}

// Cache类
/**
 * @brief 构造函数
 * 初始化
 */
Cache::Cache()
{
	state = DataState::NO_UPDATE;
	color_cache.clear();
	color_cache.clear();
}

/**
 * @brief 删除缓存
 * 更新数据库的时候删除缓存
 */
void Cache::deletecache()
{
	state = DataState::DELETED;
	color_cache.clear();
	linewidth_cache.clear();
}

/**
 * @brief 拷贝构造函数
 * 相比于默认拷贝构造,只需要数据->缓存
 * 所以不拷贝状态
 */
Cache& Cache::operator=(const Cache& cache)
{
	color_cache = cache.color_cache;
	linewidth_cache = cache.linewidth_cache;
	return *this;
}