#pragma once

#include "core/VNode.h"

namespace ikun_gui {

class Label : public VNode, public std::enable_shared_from_this<Label> {
 public:
  static std::shared_ptr<Label> create();
  Label() = default;
  ~Label() override = default;

  void init_attrs() override;

  void draw_self(SkCanvas* canvas, int64_t delta_ms) override;

 public:
  float font_size = 12.f;
  std::string font_weight = "bold";
  uint32_t color = SK_ColorGRAY;
  std::string text;
};

}  // namespace ikun_gui
