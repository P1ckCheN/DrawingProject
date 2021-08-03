// @brief: class implementation
// @copyright: Copyright 2021 by Buckychen. All rights reserved
// @birth: created by Buckychen on 2021-06-12
// @version: v2.3
// @reversion: last revised by Buckychen on 2021-06-30

#include "paintingstruct.h"

DrawingBoard::DrawingBoard():
  flag_clear_(false), 
  flag_read_xml_file_(false), 
  color_{ 0,0,0 }, 
  linewidth_(0), 
  point_begin_{ 0, 0 }, 
  point_end_{ 0, 0 },
  type_shape_(ShapeParm::TYPE_LINE), 
  type_color_(ColorParm::TYPE_RED), 
  type_linewidth_(LinewidthParm::TYPE_THIN) {
}

DrawingBoard::~DrawingBoard() {
  drawing_parm_.clear();
}

void DrawingBoard::ClearDrawing() {
  flag_clear_ = false;
  drawing_parm_.clear();
  return;
}

void DrawingBoard::CloseBoardHandle() {
  CloseHandle(read_and_exit_handle_[0]);
  CloseHandle(read_and_exit_handle_[1]);
}

Cache::Cache() {
  state_ = DataState::NO_UPDATE;
  color_cache_.clear();
  color_cache_.clear();
}

void Cache::deletecache() {
  state_ = DataState::DELETED;
  color_cache_.clear();
  linewidth_cache_.clear();
}

Cache& Cache::operator=(const Cache& cache) {
  color_cache_ = cache.color_cache_;
  linewidth_cache_ = cache.linewidth_cache_;
  return *this;
}