#include "Button.h"

#include "core/App.h"
#include "include/core/SkBlurTypes.h"
#include "include/core/SkFont.h"
#include "tools/fonts/FontToolUtils.h"

namespace ikun_gui {

std::shared_ptr<Button> Button::create() {
  return std::make_shared<Button>();
}

void Button::init_attrs() {
  VNode::init_attrs();
}

void Button::draw_self(SkCanvas *canvas, int64_t delta_ms) {
  VNode::draw_self(canvas, delta_ms);

  SkPaint borderPaint;
  borderPaint.setAntiAlias(true);
  borderPaint.setStyle(SkPaint::kStroke_Style);
  borderPaint.setStrokeWidth(2.f);
  borderPaint.setColor(SK_ColorBLUE);

  canvas->drawRect(SkRect{layout_left, layout_top, layout_left + layout_width, layout_top + layout_height}, borderPaint);
}

}  // namespace ikun_gui
