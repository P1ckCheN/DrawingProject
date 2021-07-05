// @brief: main file function declaration
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v2.6
// @reversion: last revised by Buckychen on 2021-07-02
#ifndef FREEPAINTING_H__
#define FREEPAINTING_H__

// related head files
#include "framework.h"      

// C lib
#include <windows.h>    
#include <process.h>

// C++ lib
#include <string> 
#include <vector>
#include <atomic>
#include <unordered_map>

// other lib 
#include "tinyxml/tinyxml.h" 

// within this project
#include "Resource.h" 
#include "threadpool.h"
#include "parsingxml.h"
#include "errorhandle.h"
#include "freepainting.h"
#include "paintingstruct.h"
  
#endif // !FREEPAINTING_H__