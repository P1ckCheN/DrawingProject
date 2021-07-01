// @brief: define error code 
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v2.3
// @reversion: last revised by Buckychen on 2021-06-30

#include <windows.h>

#ifndef ERRORHANDLE_H__
#define ERRORHANDLE_H__

#define SUCCESS          0    
#define ERROR_XML_PATH      1    
#define ERROR_XML_CONFIGURE    2    
#define ERROR_EMPTY_COLOR    3   
#define ERROR_EMPTY_LINEWIDTH  4  
#define ERROR_PARSING_DATA    5   
#define ERROR_REGISTER_CLASS  6    
#define ERROR_HANDLE_TIME    7    

void ErrorShow(int error_code);

#endif // !ERRORHANDLE_H__
