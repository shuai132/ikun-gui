#include "TextView.h"

#include "include/core/SkFont.h"
#include "include/core/SkStream.h"
#include "tools/fonts/FontToolUtils.h"

using namespace skia::textlayout;

namespace ikun_gui {

TextView::TextView() {
  attrs.height.percent(100);
  attrs.width.percent(100);
  attrs.color.white();

  font_collection = sk_make_sp<FontCollection>();
  font_collection->setDefaultFontManager(ToolUtils::TestFontMgr());
  font_collection->enableFontFallback();

  text_style.setFontSize(26);
  text_style.setFontStyle(SkFontStyle::Normal());
  text_style.setColor(SK_ColorBLACK);
}

std::shared_ptr<TextView> TextView::create() {
  return std::make_shared<TextView>();
}

void TextView::init_attrs() {
  VNode::init_attrs();

  if (!text_file.empty()) {
    auto data = SkData::MakeFromFileName(text_file.c_str());
    text = std::string((char *)data->data(), data->size());
  }

  touchstart = [this](const MotionEvent &e) {
    tlp_start = paragraph->getGlyphPositionAtCoordinate(e.x, e.y);
  };

  touchmove = [this](const MotionEvent &e) {
    tlp_end = paragraph->getGlyphPositionAtCoordinate(e.x, e.y);
    invalidate();
  };
  touchend = touchmove;
}

void TextView::draw_self(SkCanvas *canvas, int64_t delta_ms) {
  VNode::draw_self(canvas, delta_ms);

  if (paragraph) {
    if (layout_width != layout_width_old) {
      layout_width_old = layout_width;
      refresh_layout = true;
    }

    if (refresh_layout) {
      paragraph->layout(layout_width);
      refresh_layout = false;
    }

    int32_t position_s = std::min(tlp_start.position, tlp_end.position);
    int32_t position_e = std::max(tlp_start.position, tlp_end.position);
    auto boxes = paragraph->getRectsForRange(position_s, position_e, RectHeightStyle::kMax, RectWidthStyle::kTight);
    SkPaint paint;
    paint.setColor(0xFF888888);
    for (const auto &item : boxes) {
      canvas->drawRect(item.rect, paint);
    }

    paragraph->paint(canvas, 0, 0);
    return;
  }

  paragraph_style.setHeight(layout_height);
  paragraph_builder = ParagraphBuilder::make(paragraph_style, font_collection);

  paragraph_builder->pushStyle(text_style);
  paragraph_builder->addText(text.c_str(), text.length());

  paragraph = paragraph_builder->Build();
  paragraph->layout(layout_width);
  paragraph->paint(canvas, 0, 0);
}

}  // namespace ikun_gui
