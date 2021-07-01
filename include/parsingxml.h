// @brief: Parse xml head file
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v2.3
// @reversion: last revised by Buckychen on 2021-06-30

#ifndef PARSINGXML_H__
#define PARSINGXML_H__

#include "framework.h"

#include <windows.h>

#include <string>

#include "paintingstruct.h"

void Transform(std::string& color_value_string, int* color_value);

int ParsingDataCache(ColorParm in_color, LinewidthParm in_linewidth, int* out_color, int& out_linewidth,
  std::unordered_map<std::string, int*>& color_cache, std::unordered_map<std::string, int>& linewidth_cache);

int ReadXmlFile(DrawingBoard& board, Cache& color_linewidth_database, Cache& color_linewidth_cache);

#endif // !PARSINGXML_H__

