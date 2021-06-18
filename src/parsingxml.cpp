/**
 * \file parsingxml.cpp
 * \brief ����XML�ļ�
 *
 * ���ݴ�����ɫ��������ϸ��������xml�ļ�
 *
 * \author �����
 * \version 1.0
 * \date 2021/06/17
 */

#include "../include/parsingxml.h"

 /**
  * @brief ����ɫ�ռ��ַ���ת��������ʽ��������
  * @param [in] color_value �洢��ɫ������
  * @param [in] color_value_string �洢��ɫ���ַ���
  * @param [out] color_value �洢��ɫ������
  */
void Transform(int* color_value, std::string& color_value_string)
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
 * @param [in] in_color ѡ�еĲ˵���ɫ
 * @param [in] in_linewidth ѡ�е�������ϸ
 * @param [out] out_color ͨ��XML�ļ������õ�����ɫ����ָ��
 * @param [out] out_linewidth ͨ��XML�ļ������õ���������Сָ��
 */
int ReadXmlFile(int in_color, int in_linewidth, int* out_color, int* out_linewidth)
{
	std::string in_colorstr;
	std::string in_linewidthstr;
	switch (in_color)
	{
	case 3:
		in_colorstr = "Red";
		break;
	case 4:
		in_colorstr = "Blue";
		break;
	case 5:
		in_colorstr = "Green";
		break;
	default:
		in_colorstr = "Red";
		break;
	}
	switch (in_linewidth)
	{
	case 6:
		in_linewidthstr = "Thin";
		break;
	case 7:
		in_linewidthstr = "Medium";
		break;
	case 8:
		in_linewidthstr = "Thick";
		break;
	default:
		in_linewidthstr = "Thin";
		break;
	}

	std::string color_value_string;
	std::string linewidth_value_string;

	//���ļ�
	TiXmlDocument doc;
	if (!doc.LoadFile("xml/config.xml"))
	{
		return 0;
	}
	TiXmlElement* xml_configure = doc.RootElement();
	if (!xml_configure)
	{
		return 0;
	}
	TiXmlElement* xml_color_configure = xml_configure->FirstChildElement("color_configure");
	TiXmlElement* xml_linewidth_configure = xml_color_configure->NextSiblingElement("linewidth_configure");

	bool flag_get_color = 0;
	bool flag_get_linewidth = 0;

	// ��ȡ��ɫ��Ӧ��ֵ
	TiXmlElement* xml_color = xml_color_configure->FirstChildElement("color");
	while (xml_color != nullptr)
	{
		TiXmlElement* xml_color_name = xml_color->FirstChildElement("name");
		std::string s = xml_color_name->GetText();
		if (s == in_colorstr)
		{
			flag_get_color = 1;
			color_value_string = xml_color_name->NextSiblingElement("value")->GetText();
			Transform(out_color, color_value_string);
		}
		xml_color = xml_color->NextSiblingElement();
	}

	// ��ȡ������ϸ��Ӧ��ֵ
	TiXmlElement* xml_linewidth = xml_linewidth_configure->FirstChildElement("linewidth");
	while (xml_linewidth != nullptr)
	{
		TiXmlElement* xml_linewidth_name = xml_linewidth->FirstChildElement("name");
		std::string s = xml_linewidth_name->GetText();
		if (s == in_linewidthstr)
		{
			flag_get_linewidth = 1;
			linewidth_value_string = xml_linewidth_name->NextSiblingElement("value")->GetText();
			*out_linewidth = std::stoi(linewidth_value_string);
		}
		xml_linewidth = xml_linewidth->NextSiblingElement();
	}

	// �쳣�ж�
	if (!flag_get_color || !flag_get_linewidth)
	{
		return 0;
	}
	return 1;
}