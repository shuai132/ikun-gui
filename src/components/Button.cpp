#include "Button.h"

namespace ikun_gui {

std::shared_ptr<Button> Button::create() {
  return std::make_shared<Button>();
}

void Button::init_attrs() {
  if (!attrs.color.is_set) {
    attrs.color.white();
  }
  if (!attrs.border_radius.is_set) {
    attrs.border_radius(12, 12);
  }
  VNode::init_attrs();
}

void Button::draw_self(SkCanvas *canvas, int64_t delta_ms) {
  VNode::draw_self(canvas, delta_ms);
}

}  // namespace ikun_gui
