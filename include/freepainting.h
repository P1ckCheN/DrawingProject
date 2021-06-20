/**
 * \file freepainting.h
 * \brief ������ͷ�ļ�
 *
 * ����ṹ��
 *
 * \author �����
 * \version 1.0
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
	TYPE_LINE,						///< ��״��ֱ��	
	TYPE_RECT,						///< ��״������
	TYPE_ELLIPSE,					///< ��״����Բ
};

enum ColorParm
{
	TYPE_RED,						///< ��ɫ����ɫ
	TYPE_BLUE,						///< ��ɫ����ɫ
	TYPE_GREEN,						///< ��ɫ����ɫ
};

enum LinewidthParm
{
	TYPE_THIN,						///< ��ϸ�ȣ�ϸ
	TYPE_MEDIUM,					///< ��ϸ�ȣ���
	TYPE_THICK						///< ��ϸ�ȣ���
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