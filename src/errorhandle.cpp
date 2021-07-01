// @brief: error code display
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v2.5
// @reversion: last revised by Buckychen on 2021-07-01

#include "errorhandle.h"

void ErrorShow(int error_code)
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
    case ERROR_HANDLE_TIME:
      MessageBox(NULL, TEXT("Time out, Please check the program!"),
        szAppName, MB_ICONERROR);
      break;
    case ERROR_HANDLE_WAITOBJECT:
      MessageBox(NULL, TEXT("Get event failed, please check the event handle!"),
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