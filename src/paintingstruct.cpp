#include "paintingstruct.h"

// DrawingBoard��
/**
 * @brief ���캯��
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
 * @brief ��������
 */
DrawingBoard::~DrawingBoard()
{
	drawing_parm.clear();
}

/**
 * @brief �����Ļ����
 * �Ҽ�����ʵ��
 */
void DrawingBoard::ClearDrawing()
{
	flag_clear = false;
	drawing_parm.clear();
	return;
}

// Cache��
/**
 * @brief ���캯��
 * ��ʼ��
 */
Cache::Cache()
{
	state = DataState::NO_UPDATE;
	color_cache.clear();
	color_cache.clear();
}

/**
 * @brief ɾ������
 * �������ݿ��ʱ��ɾ������
 */
void Cache::deletecache()
{
	state = DataState::DELETED;
	color_cache.clear();
	linewidth_cache.clear();
}

/**
 * @brief �������캯��
 * �����Ĭ�Ͽ�������,ֻ��Ҫ����->����
 * ���Բ�����״̬
 */
Cache& Cache::operator=(const Cache& cache)
{
	color_cache = cache.color_cache;
	linewidth_cache = cache.linewidth_cache;
	return *this;
}