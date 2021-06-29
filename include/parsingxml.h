/**
 * \file parsingxml.h
 * \brief ����XML����ͷ�ļ�
 *
 * ����ͷ�ļ�
 *
 * \author �����
 * \version 1.2
 * \date 2021/06/17
 */

#ifndef PARSINGXML_H__
#define PARSINGXML_H__

#include "framework.h"

#include <windows.h>

#include <string>

#include "paintingstruct.h"

void Transform(int* color_value, std::string& color_value_string);

int ParsingDataCache(ColorParm in_color, LinewidthParm in_linewidth, int* out_color, int& out_linewidth,
	std::unordered_map<std::string, int*>& color_cache, std::unordered_map<std::string, int>& linewidth_cache);

int ParsingDataAtomic(ColorParm in_color, LinewidthParm in_linewidth, int* out_color, int& out_linewidth,
	std::unordered_map<std::string, std::atomic_int*>& color_cache, std::unordered_map<std::string, std::atomic_int>& linewidth_cache);

int ReadXmlFile(DrawingBoard& board, Cache& color_linewidth_database, Cache& color_linewidth_cache);

#endif // !PARSINGXML_H__

