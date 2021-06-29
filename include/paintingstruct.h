#ifndef PAINTINGSTRUCT_H__
#define PAINTINGSTRUCT_H__

#include <windows.h>

#include <vector>
#include <unordered_map>

#include "errorhandle.h"
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
	NO_UPDATE,													///< δ����
	UPDATEED,													///< �Ѹ���
	DELETED														///< ��ɾ��
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
	~Cache() {}													///< ��������
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
	DrawingBoard();
	~DrawingBoard();
	void Drawing(HWND hwnd, HDC hdc, Cache& color_linewidth_database, Cache& color_linewidth_cache);
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

#endif // ! PAINTINGSTRUCT_H__
