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

#include "Resource.h"
#include "parsingxml.h"

// ��������ṹ��
enum Parm
{
	TYPE_LINE,						///< ��״��ֱ��	
	TYPE_RECT,						///< ��״������
	TYPE_ELLIPSE,					///< ��״����Բ
	TYPE_RED,						///< ��ɫ����ɫ
	TYPE_BLUE,						///< ��ɫ����ɫ
	TYPE_GREEN,						///< ��ɫ����ɫ
	TYPE_THIN,						///< ��ϸ�ȣ�ϸ
	TYPE_MEDIUM,					///< ��ϸ�ȣ���
	TYPE_THICK						///< ��ϸ�ȣ���
};

#endif