/**
 * \file freepainting.h
 * \brief ������ͷ�ļ�
 *
 * ����ṹ�������ȫ�ֱ���
 *
 * \author �����
 * \version 1.2
 * \date 2021/06/17
 */

#ifndef FREEPAINTING_H__
#define FREEPAINTING_H__


#include "framework.h"
#include "Resource.h"
#include "parsingxml.h"

 // ��������ṹ��
enum ShapeParm
{	
	TYPE_LINE,													///< ��״��ֱ��	
	TYPE_RECT,													///< ��״������
	TYPE_ELLIPSE,												///< ��״����Բ
};

enum ColorParm
{
	TYPE_RED,													///< ��ɫ����ɫ
	TYPE_BLUE,													///< ��ɫ����ɫ
	TYPE_GREEN,													///< ��ɫ����ɫ
};

enum LinewidthParm
{
	TYPE_THIN,													///< ��ϸ�ȣ�ϸ
	TYPE_MEDIUM,												///< ��ϸ�ȣ���
	TYPE_THICK													///< ��ϸ�ȣ���
};

struct DrawParm	 
{
	POINT ptBegin;												///< ��ʼ��
	POINT ptEnd;												///< ��ֹ��
	ShapeParm shape_parm;										///< ��״
	std::vector<int> color_parm;								///< ��ɫ
	int LinewidthParm;											///< �������
};

// ����ʱ����ȫ�ֱ���
static bool flag_clear;											///< �������
static bool flag_readxmlfile;									///< �Ƿ��ȡ�ļ�

static POINT ptShapeBegin;										///< ��ʼ��
static POINT ptShapeEnd;										///< ��ֹ��

static ShapeParm type_shape;									///< ��״����
static ColorParm type_color;									///< ��ɫ����
static LinewidthParm type_linewidth;							///< �����������
static std::vector<DrawParm> draw_parm;							///< ������ʷ��������

static int color[3];											///< ��ɫֵ
static int linewidth;											///< �������ֵ

static std::unordered_map<std::string, int*> color_cache;		///< ��ϣ����Ӧ��ɫӳ��
static std::unordered_map<std::string, int> linewidth_cache;	///< ��ϣ����Ӧ�������ӳ��

// ��������
VOID SaveDrawing(HWND hwnd, HDC hdc, POINT ptBegin, POINT ptEnd,
	std::vector<DrawParm>& draw_parm, ShapeParm type_shape, ColorParm type_color, LinewidthParm type_linewidth);

VOID ClearDrawing(std::vector<DrawParm>& draw_parm);

VOID ThreadReadXmlFile(PVOID pvoid);

#endif // !FREEPAINTING_H__