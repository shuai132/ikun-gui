#pragma once

#include "core/VNode.h"

namespace ikun_gui {

class Label : public VNode {
 public:
  static std::shared_ptr<Label> create();
  Label() = default;
  ~Label() override = default;

  void init_attrs() override;

  void draw_self(SkCanvas* canvas, int64_t delta_ms) override;

 public:
  float font_size = 12.f;
  std::string font_weight = "bold";
  Color color;
  bool bold = false;
  std::string text;
};

}  // namespace ikun_gui
