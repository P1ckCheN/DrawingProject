/**
 * \file errorhandle.h
 * \brief 异常处理头文件
 *
 * 定义异常编号
 *
 * \author 陈瑞佳
 * \version 1.0
 * \date 2021/06/21
 */

#ifndef ERRORHANDLE_H__
#define ERRORHANDLE_H__

#define SUCCESS 0						///< 成功返回	
#define ERROR_XML_PATH 1				///< 路径错误
#define ERROR_XML_CONFIGURE 2			///< 配置信息错误
#define ERROR_EMPTY_COLOR 3				///< 无法读取色彩信息
#define ERROR_EMPTY_LINEWIDTH 4			///< 无法读取线条宽度信息	
#define ERROR_PARSING_DATA 5			///< 解析文件错误
#define ERROR_REGISTER_CLASS 6			///< 注册窗口错误

VOID ErrorShow(int error_code);

#endif // !ERRORHANDLE_H__
