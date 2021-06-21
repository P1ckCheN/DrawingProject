/**
 * \file errorhandle.cpp
 * \brief 异常处理文件
 *
 * 处理异常，包括打印错误码等函数。
 *
 * \author 陈瑞佳
 * \version 1.0
 * \date 2021/06/21
 */

#include "framework.h"

/**
 * @brief 打印窗体错误
 * @param [in] error_code				传入错误码
 */
VOID ErrorShow(int error_code)
{	
	TCHAR szAppName[] = TEXT("FreePainting");
	switch (error_code)
	{
		case SUCCESS:
			return;
		case ERROR_XML_PATH:
			MessageBox(NULL, TEXT("Cannot find xml file, error path!"),
				szAppName, MB_ICONERROR);
			break;
		case ERROR_XML_CONFIGURE:
			MessageBox(NULL, TEXT("Configure file error!"),
				szAppName, MB_ICONERROR);
			break;
		case ERROR_EMPTY_COLOR:
			MessageBox(NULL, TEXT("Cannot load color information!"),
				szAppName, MB_ICONERROR);
			break;
		case ERROR_EMPTY_LINEWIDTH:
			MessageBox(NULL, TEXT("Cannot load linewidth information!"),
				szAppName, MB_ICONERROR);
			break;
		case ERROR_PARSING_DATA:
			MessageBox(NULL, TEXT("Cannot parse parameters!"),
				szAppName, MB_ICONERROR);
			break;
		case ERROR_REGISTER_CLASS:
			MessageBox(NULL, TEXT("Error in registing class, Program requires Windows NT!"),
				szAppName, MB_ICONERROR);
			break;
		default:
			break;
	}
	if (error_code != SUCCESS)
	{
		exit(-1);
	}
	return;
}