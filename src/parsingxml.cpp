// @brief: Parse xml file
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v2.3
// @reversion: last revised by Buckychen on 2021-07-01

#include "parsingxml.h"


// Transform the data type (string to int)
void Transform(std::string& color_value_string, int* color_value) {
  for (int i = 0; i < 3; i++)
  {
    color_value[i] = 0;
  }
  int idx = 0;
  for (unsigned int i = 1; i < color_value_string.size() - 1; i++)
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

// Parse data according to user's choice 
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

  // File don't contain message in menu list
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

//int count = 0;
int ReadXmlFile(DrawingBoard* board)
{
  //Varifying the timer 
  /*std::ofstream outfile;
  SYSTEMTIME sys;
  outfile.open("doc/record.txt", std::ios::app);
  outfile << "Times " << count++ << " :"<< std::endl;
  GetLocalTime(&sys);
  outfile << sys.wMinute << "'" << sys.wSecond << std::endl << std::endl;
  outfile.close();*/

  board->color_linewidth_database.state = DataState::NO_UPDATE;

  std::string color_value_string;
  std::string linewidth_value_string;
  int* color_transform = new int[3];

  TiXmlDocument doc;
  if (!doc.LoadFile("xml/config.xml"))
  {
    return ERROR_XML_PATH;
  }
  TiXmlElement* xml_configure = doc.RootElement();
  if (!xml_configure)
  {
    return ERROR_XML_CONFIGURE;
  }
  TiXmlElement* xml_color_configure = xml_configure->FirstChildElement("color_configure");
  TiXmlElement* xml_linewidth_configure = xml_color_configure->NextSiblingElement("linewidth_configure");

  bool flag_get_color = 0;
  bool flag_get_linewidth = 0;

  // Find the color information
  TiXmlElement* xml_color = xml_color_configure->FirstChildElement("color");
  while (xml_color != nullptr)
  {
    TiXmlElement* xml_color_name = xml_color->FirstChildElement("name");
    std::string s = xml_color_name->GetText();

    color_value_string = xml_color_name->NextSiblingElement("value")->GetText();
    Transform(color_value_string, color_transform);

    board->color_linewidth_database.color_cache[s] = new int[3];
    for (int i = 0; i < 3; i++)
    {
      board->color_linewidth_database.color_cache[s][i] = color_transform[i];
    }

    xml_color = xml_color->NextSiblingElement();
  }

  // Find the linewidth information
  TiXmlElement* xml_linewidth = xml_linewidth_configure->FirstChildElement("linewidth");
  while (xml_linewidth != nullptr)
  {
    TiXmlElement* xml_linewidth_name = xml_linewidth->FirstChildElement("name");
    std::string s = xml_linewidth_name->GetText();
    board->color_linewidth_database.linewidth_cache[s] = std::stoi(xml_linewidth_name->NextSiblingElement("value")->GetText());
    xml_linewidth = xml_linewidth->NextSiblingElement();
  }

  // Judge the error
  if (board->color_linewidth_database.color_cache.empty())
  {
    return ERROR_EMPTY_COLOR;
  }
  if (board->color_linewidth_database.linewidth_cache.empty())
  {
    return ERROR_EMPTY_LINEWIDTH;
  }

  // if the data are updated, delete the copy
  board->color_linewidth_database.state = DataState::UPDATEED;
  board->color_linewidth_cache.state = DataState::DELETED;
  board->color_linewidth_cache.deletecache();
  board->SetRead(true);
  return SUCCESS;
}