// @brief: class implementation
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v2.3
// @reversion: last revised by Buckychen on 2021-06-30

#include "paintingstruct.h"

DrawingBoard::DrawingBoard():
  flag_clear(false), flag_read_xml_file(false), color{ 0,0,0 }, linewidth(0), point_begin{ 0, 0 }, point_end{ 0, 0 },
  type_shape(ShapeParm::TYPE_LINE), type_color(ColorParm::TYPE_RED), type_linewidth(LinewidthParm::TYPE_THIN) {
}

DrawingBoard::~DrawingBoard() {
  drawing_parm.clear();
}

void DrawingBoard::ClearDrawing() {
  flag_clear = false;
  drawing_parm.clear();
  return;
}

void DrawingBoard::CloseBoardHandle() {
  CloseHandle(read_and_exit_handle[0]);
  CloseHandle(read_and_exit_handle[1]);
}
Cache::Cache() {
  state = DataState::NO_UPDATE;
  color_cache.clear();
  color_cache.clear();
}

void Cache::deletecache() {
  state = DataState::DELETED;
  color_cache.clear();
  linewidth_cache.clear();
}

Cache& Cache::operator=(const Cache& cache) {
  color_cache = cache.color_cache;
  linewidth_cache = cache.linewidth_cache;
  return *this;
}