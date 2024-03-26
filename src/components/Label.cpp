#include "Label.h"

#include "core/App.h"
#include "include/core/SkFont.h"
#include "tools/fonts/FontToolUtils.h"

namespace ikun_gui {

std::shared_ptr<Label> Label::create() {
  return std::make_shared<Label>();
}

void Label::init_attrs() {
  if (attrs.height.type == AttrValue::Type::unset) {
    attrs.height.percent(100);
  }
  if (attrs.width.type == AttrValue::Type::unset) {
    attrs.width.percent(100);
  }
  enable_touch = false;
  VNode::init_attrs();
}

void Label::draw_self(SkCanvas *canvas, int64_t delta_ms) {
  VNode::draw_self(canvas, delta_ms);

  SkFont font(runtime->app()->font_manager.typeface, font_size);
  font.setSubpixel(true);
  font.setSize(font_size);
  SkPaint paint;
  if (color.is_set) {
    paint.setColor(color.value);
  }
  if (bold) {
    font.setEmbolden(true);
  }

  SkRect bounds;
  font.measureText(text.data(), text.length(), SkTextEncoding::kUTF8, &bounds, &paint);

  // clang-format off
  canvas->drawSimpleText(text.data(),
                         text.length(),
                         SkTextEncoding::kUTF8,
                         layout_left + (layout_width - bounds.width())/ 2,
                         layout_top + layout_height - (layout_height - bounds.height())/ 2,
                         font,
                         paint
                         );
  // clang-format on
}

}  // namespace ikun_gui
