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

#include "framework.h"			// ���ͷ�ļ�

#include <windows.h>			// C��
#include <process.h>

#include <string>				// C++��
#include <vector>
#include <atomic>
#include <unordered_map>

#include "tinyxml/tinyxml.h"	// �������.h

#include "Resource.h"			// ����Ŀ�ڵ�.h
#include "parsingxml.h"
#include "errorhandle.h"
#include "freepainting.h"
#include "paintingstruct.h"
	
// ��������
unsigned int WINAPI ThreadReadXmlFile(void* p_parm);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#endif // !FREEPAINTING_H__