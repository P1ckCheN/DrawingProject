/**
 * \file freepainting.h
 * \brief ������ͷ�ļ�
 *
 * ����ṹ�������ȫ�ֱ���
 *
 * \author �����
 * \version 2.2
 * \date 2021/06/24
 */

#ifndef FREEPAINTING_H__
#define FREEPAINTING_H__

#include "framework.h"
#include "Resource.h"

 // ��ͼ����
enum class ShapeParm
{	
	TYPE_LINE,													///< ��״��ֱ��	
	TYPE_RECT,													///< ��״������
	TYPE_ELLIPSE												///< ��״����Բ
};

enum class ColorParm
{
	TYPE_RED,													///< ��ɫ����ɫ
	TYPE_BLUE,													///< ��ɫ����ɫ
	TYPE_GREEN													///< ��ɫ����ɫ
};

enum class LinewidthParm
{
	TYPE_THIN,													///< ��ϸ�ȣ�ϸ
	TYPE_MEDIUM,												///< ��ϸ�ȣ���
	TYPE_THICK													///< ��ϸ�ȣ���
};

// �洢�ṹ��
struct DrawingParm	 
{
	POINT point_begin;											///< ��ʼ��
	POINT point_end;											///< ��ֹ��
	ShapeParm shape_parm;										///< ��״
	std::vector<int> color_parm;								///< ��ɫ
	int linewidth_parm;											///< �������
};

enum class DataState
{
	NO_UPDATE = 0,												///< δ����
	UPDATEED = 1,												///< �Ѹ���
	DELETED = 2													///< ��ɾ��
};

// ģ��洢��
class Cache
{
public:
	// ��Ա����
	DataState state;											///< ��ǰ״̬
	std::unordered_map<std::string, int*> color_cache;			///< ��ɫ�洢
	std::unordered_map<std::string, int > linewidth_cache;		///< ������ȴ洢

	// ��Ա����
	Cache();													///< ���캯��
	void deletecache();											///< ɾ������
	Cache& operator=(const Cache& cache);						///< ���ظ�ֵ�����
};



// ������
class DrawingBoard
{
private:
	// ��Ա����
	bool flag_clear;										    ///< �������
	bool flag_read_xml_file;									///< �Ƿ��Ѿ���ȡ�ļ�

	int color[3];												///< ���������ɫ
	int linewidth;												///< ��������������

	POINT point_begin;											///< ������������ʼ��
	POINT point_end;											///< ������������ֹ��

	ShapeParm type_shape;										///< ��������
	ColorParm type_color;										///< ��ɫ����
	LinewidthParm type_linewidth;								///< �����������
	std::vector<DrawingParm> drawing_parm;						///< �����洢�ṹ��

public:
	// ��Ա����
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

// ȫ�ֱ���
static DrawingBoard board;											///< �������
static POINT point_curr_begin;										///< ��ʼ��
static POINT point_curr_end;										///< ��ֹ��
static Cache color_linewidth_cache;									///< ����
static Cache color_linewidth_database;								///< ���ݿ�
	
// ��������
void ThreadReadXmlFile(DrawingBoard& board);

void ReadXmlFile1(DrawingBoard& board);

void Transform(int* color_value, std::string& color_value_string);

int ParsingDataCache(ColorParm in_color, LinewidthParm in_linewidth, int* out_color, int& out_linewidth,
	std::unordered_map<std::string, int*>& color_cache, std::unordered_map<std::string, int>& linewidth_cache);

int ParsingDataAtomic(ColorParm in_color, LinewidthParm in_linewidth, int* out_color, int& out_linewidth,
	std::unordered_map<std::string, std::atomic_int*>& color_cache, std::unordered_map<std::string, std::atomic_int>& linewidth_cache);

int ReadXmlFile(DrawingBoard& board);

#endif // !FREEPAINTING_H__