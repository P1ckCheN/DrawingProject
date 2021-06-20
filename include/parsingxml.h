/**
 * \file parsingxml.h
 * \brief 解析XML函数头文件
 *
 * 头文件
 *
 * \author 陈瑞佳
 * \version 1.0
 * \date 2021/06/17
 */

#ifndef PARSINGXML_H__
#define PARSINGXML_H__

#include "framework.h"
#include "freepainting.h"
#include "tinyxml/tinyxml.h"

void Transform(int* color_value, std::string& color_value_string);

int ReadXmlFile(int in_color, int in_linewidth, int* out_color, int* out_linewidth);

#endif // !PARSINGXML_H__

