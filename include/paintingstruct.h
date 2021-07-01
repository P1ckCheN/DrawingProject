// @brief: class declaration
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v2.3
// @reversion: last revised by Buckychen on 2021-06-30

#ifndef PAINTINGSTRUCT_H__
#define PAINTINGSTRUCT_H__

#include <windows.h>

#include <vector>
#include <unordered_map>

#include "errorhandle.h"

// enum type
enum class ShapeParm {
  TYPE_LINE,                         
  TYPE_RECT,                          
  TYPE_ELLIPSE                        
};

enum class ColorParm {
  TYPE_RED,                           
  TYPE_BLUE,                          
  TYPE_GREEN                          
};

enum class LinewidthParm {
  TYPE_THIN,                          
  TYPE_MEDIUM,                        
  TYPE_THICK                         
};

enum class DataState
{
  NO_UPDATE,                         
  UPDATEED,                         
  DELETED                            
};

// store history parameters
struct DrawingParm {
  POINT point_begin;                 
  POINT point_end;                 
  ShapeParm shape_parm;               
  std::vector<int> color_parm;        
  int linewidth_parm;                
};

// Store the data of parsing xml files
// Example:
//  Cache* cache_sample = new Cache();
//  Cache* cache_sample_copy;
//  cache_sample_copy = cache_sample;
//  cache_sample.deletecache();
class Cache {
public:
  DataState state;                     
  std::unordered_map<std::string, int*> color_cache;     
  std::unordered_map<std::string, int > linewidth_cache;  

public:
  Cache();                        
  ~Cache() {}                         
  void deletecache();                     
  Cache& operator=(const Cache& cache);           
};

// Operate the draw object, and store information
// inlcuding drawing parameters, drawing points, clearing, setting color...
// Example:
//  DrawingBoard* board = new DrawingBoard();
//  board.SetType(ShapeParm::TYPE_LINE);
//  board.SetColor(ColorParm::TYPE_RED);
//  board.SetLinewidthParm(LinewidhtParm::TYPE_THIN);
//  board.ClearDrawing();
//  Cache color_linewidth_cache, color_linewidth_database;
//  ...
//  board.Drawing(hwnd, hdc, database, cache);
class DrawingBoard {
private:
  int color[3];
  int linewidth;
  bool flag_clear;                         
  bool flag_read_xml_file;                                  
  POINT point_begin;                       
  POINT point_end;                        
  ShapeParm type_shape;                    
  ColorParm type_color;                     
  LinewidthParm type_linewidth;            
  std::vector<DrawingParm> drawing_parm;   

public:
  DrawingBoard();
  ~DrawingBoard();
  void ClearDrawing();
  inline bool GetClear() { return flag_clear; }
  inline void SetClear() { flag_clear = true; }
  inline void SetType(ShapeParm shape) { type_shape = shape; }
  inline void SetColor(ColorParm color) { type_color = color; }
  inline void SetRead(bool flag) { flag_read_xml_file = flag; }
  inline void SetLinewidth(LinewidthParm linewidth) { 
    type_linewidth = linewidth; 
  }
  inline void SetCurrPoint(POINT point_curr_begin, POINT point_curr_end) {
    point_begin = point_curr_begin;
    point_end = point_curr_end;
  }
  void Drawing(HWND hwnd, HDC hdc, Cache& color_linewidth_database, Cache& color_linewidth_cache);
};

#endif // ! PAINTINGSTRUCT_H__
