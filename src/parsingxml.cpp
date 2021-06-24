/**
 * \file parsingxml.cpp
 * \brief 解析XML文件
 *
 * 根据传入颜色和线条粗细参数解析xml文件
 *
 * \author 陈瑞佳
 * \version 2.0
 * \date 2021/06/17
 */

//#include "../include/parsingxml.h"
#include "framework.h"

 /**
  * @brief 将颜色空间字符串转成数字形式存入数组
  * @param [in] color_value 存储颜色的数组
  * @param [in] color_value_string 存储颜色的字符串
  * @param [out] color_value 存储颜色的数组
  */
VOID Transform(int* color_value, std::string& color_value_string)
{
	for (int i = 0; i < 3; i++)
	{
		color_value[i] = 0;
	}
	int idx = 0;
	for (int i = 1; i < color_value_string.size() - 1; i++)
	{
		if (color_value_string[i] != ',')
		{
			color_value[idx] *= 10;
			color_value[idx] += color_value_string[i] - '0';
		}
		else
			idx++;
	}
	return;
}



/**
 * @brief 解析XML文件
 * @param [in] in_color				通过菜单选择的颜色
 * @param [in] in_linewidth			通过菜单选择的线条宽度
 * @param [in] color_cache			颜色空间缓存映射
 * @param [in] linewidth_cache		线条宽度缓存映射
 * @param [out] out_color			通过映射得到颜色值
 * @param [out] out_linewidthline	通过映射得到线条宽度值
 */
int ParsingDataCache(ColorParm in_color, LinewidthParm in_linewidth, int* out_color, int& out_linewidth, 
	std::unordered_map<std::string, int*>& color_cache, std::unordered_map<std::string, int>& linewidth_cache)
{
	std::string in_colorstr;
	std::string in_linewidthstr;
	switch (in_color)
	{
	case ColorParm::TYPE_RED:
		in_colorstr = "Red";
		break;
	case ColorParm::TYPE_BLUE:
		in_colorstr = "Blue";
		break;
	case ColorParm::TYPE_GREEN:
		in_colorstr = "Green";
		break;
	default:
		in_colorstr = "Red";
		break;
	}
	switch (in_linewidth)
	{
	case LinewidthParm::TYPE_THIN:
		in_linewidthstr = "Thin";
		break;
	case LinewidthParm::TYPE_MEDIUM:
		in_linewidthstr = "Medium";
		break;
	case LinewidthParm::TYPE_THICK:
		in_linewidthstr = "Thick";
		break;
	default:
		in_linewidthstr = "Thin";
		break;
	}

	// 如果更新文件中不包含所需信息，可以返回错误码，也可以使用默认值
	if (color_cache.find(in_colorstr) == color_cache.end() 
		|| linewidth_cache.find(in_linewidthstr) == linewidth_cache.end())
	{
		return ERROR_PARSING_DATA;
	}

	for (int i = 0; i < 3; i++)
	{
		out_color[i] = color_cache[in_colorstr][i];
	}
	out_linewidth = linewidth_cache[in_linewidthstr];

	return SUCCESS;
}/**
 * @brief 解析XML文件
 * @param [in] in_color				通过菜单选择的颜色
 * @param [in] in_linewidth			通过菜单选择的线条宽度
 * @param [in] color_cache			颜色空间缓存映射
 * @param [in] linewidth_cache		线条宽度缓存映射
 * @param [out] out_color			通过映射得到颜色值
 * @param [out] out_linewidthline	通过映射得到线条宽度值
 */
int ParsingDataAtomic(ColorParm in_color, LinewidthParm in_linewidth, int* out_color, int& out_linewidth, 
	std::unordered_map<std::string, std::atomic_int*>& color_cache, std::unordered_map<std::string, std::atomic_int>& linewidth_cache)
{
	std::string in_colorstr;
	std::string in_linewidthstr;
	switch (in_color)
	{
	case ColorParm::TYPE_RED:
		in_colorstr = "Red";
		break;
	case ColorParm::TYPE_BLUE:
		in_colorstr = "Blue";
		break;
	case ColorParm::TYPE_GREEN:
		in_colorstr = "Green";
		break;
	default:
		in_colorstr = "Red";
		break;
	}
	switch (in_linewidth)
	{
	case LinewidthParm::TYPE_THIN:
		in_linewidthstr = "Thin";
		break;
	case LinewidthParm::TYPE_MEDIUM:
		in_linewidthstr = "Medium";
		break;
	case LinewidthParm::TYPE_THICK:
		in_linewidthstr = "Thick";
		break;
	default:
		in_linewidthstr = "Thin";
		break;
	}

	// 如果更新文件中不包含所需信息，可以返回错误码，也可以使用默认值
	if (color_cache.find(in_colorstr) == color_cache.end() 
		|| linewidth_cache.find(in_linewidthstr) == linewidth_cache.end())
	{
		return ERROR_PARSING_DATA;
	}

	for (int i = 0; i < 3; i++)
	{
		out_color[i] = color_cache[in_colorstr][i];
	}
	out_linewidth = linewidth_cache[in_linewidthstr];

	return SUCCESS;
}