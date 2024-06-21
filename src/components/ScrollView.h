#pragma once

#include "core/VNode.h"

namespace ikun_gui {

class ScrollView : public VNode {
 public:
  static std::shared_ptr<ScrollView> create();
  ScrollView();
  ~ScrollView() override = default;

  void init_attrs() override;

  void draw_self(SkCanvas* canvas, int64_t delta_ms) override;

 protected:
  virtual float content_height();
  virtual float content_width();

 protected:
  float scroll_pos_top = 0;
  float scroll_pos_top_old = 0;
  float scroll_pos_left = 0;
  float scroll_pos_left_old = 0;
};

}  // namespace ikun_gui
