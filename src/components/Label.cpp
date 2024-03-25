#include "Label.h"

#include "core/App.h"
#include "include/core/SkFont.h"
#include "tools/fonts/FontToolUtils.h"

namespace ikun_gui {

std::shared_ptr<Label> Label::create() {
  return std::make_shared<Label>();
}

void Label::init_attrs() {
  attrs.height.percent(100);
  attrs.width.percent(100);
  VNode::init_attrs();
}

void Label::draw_self(SkCanvas *canvas, int64_t delta_ms) {
  VNode::draw_self(canvas, delta_ms);

  SkFont font(runtime->app()->font_manager.typeface, font_size);
  font.setSubpixel(true);
  font.setSize(font_size);

  // todo:
  float left = YGNodeLayoutGetLeft(yoga_node);
  float top = YGNodeLayoutGetTop(yoga_node);
  float width = YGNodeLayoutGetWidth(yoga_node);
  float height = YGNodeLayoutGetHeight(yoga_node);
  SkPaint paint;
  paint.setColor(color);
  canvas->drawSimpleText(text.c_str(), text.length(), SkTextEncoding::kUTF8, left + width / 2 - (font_size * text.length() / 2), top + height / 2,
                         font, paint);
}

}  // namespace ikun_gui
