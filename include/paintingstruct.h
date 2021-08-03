// @brief: class declaration
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v2.6
// @reversion: last revised by Buckychen on 2021-07-02

#ifndef PAINTINGSTRUCT_H__
#define PAINTINGSTRUCT_H__

#include <windows.h>
#include <vector>
#include <unordered_map>
#include "errorhandle.h"

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

enum class DataState {
  NO_UPDATE,                         
  UPDATEED,                         
  DELETED                            
};

struct DrawingParm {
  POINT point_begin;                 
  POINT point_end;                 
  ShapeParm shape_parm;               
  std::vector<int> color_parm;        
  int linewidth_parm;                
};

class Cache {
 public:
  DataState state_;                     
  std::unordered_map<std::string, int*> color_cache_;     
  std::unordered_map<std::string, int > linewidth_cache_;  

  Cache();                        
  ~Cache() {}                         
  void deletecache();                     
  Cache& operator=(const Cache& cache);           
};

class DrawingBoard {
 public:
  DrawingBoard();
  ~DrawingBoard();
  void ClearDrawing();
  void CloseBoardHandle();
  void Drawing(HWND hwnd, HDC hdc);

  inline bool GetClear() { return flag_clear_; }
  inline void SetClear() { flag_clear_ = true; }
  inline void SetType(ShapeParm shape) { type_shape_ = shape; }
  inline void SetColor(ColorParm color) { type_color_ = color; }
  inline void SetRead(bool flag) { flag_read_xml_file_ = flag; }
  inline void SetLinewidth(LinewidthParm linewidth) { type_linewidth_ = linewidth; }
  inline void SetCurrPoint() {
    point_begin_ = point_curr_begin_;
    point_end_ = point_curr_end_;
  }

  POINT point_curr_end_;
  POINT point_curr_begin_;
  Cache color_linewidth_cache_;
  Cache color_linewidth_database_;
  HANDLE read_and_exit_handle_[2];

 private:
  int color_[3];
  int linewidth_;
  bool flag_clear_;
  bool flag_read_xml_file_;
  POINT point_begin_;
  POINT point_end_;
  ShapeParm type_shape_;
  ColorParm type_color_;
  LinewidthParm type_linewidth_;
  std::vector<DrawingParm> drawing_parm_;
};

#endif // ! PAINTINGSTRUCT_H__
