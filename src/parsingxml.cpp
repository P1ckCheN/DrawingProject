/**
 * \file parsingxml.cpp
 * \brief ����XML�ļ�
 *
 * ���ݴ�����ɫ��������ϸ��������xml�ļ�
 *
 * \author �����
 * \version 2.0
 * \date 2021/06/17
 */

//#include "../include/parsingxml.h"
#include "framework.h"

 /**
  * @brief ����ɫ�ռ��ַ���ת��������ʽ��������
  * @param [in] color_value �洢��ɫ������
  * @param [in] color_value_string �洢��ɫ���ַ���
  * @param [out] color_value �洢��ɫ������
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
 * @brief ����XML�ļ�
 * @param [in] in_color				ͨ���˵�ѡ�����ɫ
 * @param [in] in_linewidth			ͨ���˵�ѡ����������
 * @param [in] color_cache			��ɫ�ռ仺��ӳ��
 * @param [in] linewidth_cache		������Ȼ���ӳ��
 * @param [out] out_color			ͨ��ӳ��õ���ɫֵ
 * @param [out] out_linewidthline	ͨ��ӳ��õ��������ֵ
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

	// ��������ļ��в�����������Ϣ�����Է��ش����룬Ҳ����ʹ��Ĭ��ֵ
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
 * @brief ����XML�ļ�
 * @param [in] in_color				ͨ���˵�ѡ�����ɫ
 * @param [in] in_linewidth			ͨ���˵�ѡ����������
 * @param [in] color_cache			��ɫ�ռ仺��ӳ��
 * @param [in] linewidth_cache		������Ȼ���ӳ��
 * @param [out] out_color			ͨ��ӳ��õ���ɫֵ
 * @param [out] out_linewidthline	ͨ��ӳ��õ��������ֵ
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

	// ��������ļ��в�����������Ϣ�����Է��ش����룬Ҳ����ʹ��Ĭ��ֵ
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