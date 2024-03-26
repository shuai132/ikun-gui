#pragma once

#include "core/VNode.h"

namespace ikun_gui {

class Button : public VNode {
 public:
  static std::shared_ptr<Button> create();
  Button() = default;
  ~Button() override = default;

  void init_attrs() override;

  void draw_self(SkCanvas* canvas, int64_t delta_ms) override;
};

}  // namespace ikun_gui
