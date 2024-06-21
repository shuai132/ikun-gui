#pragma once

#include "ScrollView.h"
#include "core/VNode.h"
#include "modules/skparagraph/include/Paragraph.h"
#include "modules/skparagraph/include/ParagraphStyle.h"
#include "modules/skparagraph/src/ParagraphBuilderImpl.h"

namespace ikun_gui {

class TextView : public ScrollView {
 public:
  static std::shared_ptr<TextView> create();
  TextView();
  ~TextView() override = default;

  void init_attrs() override;

  void draw_self(SkCanvas* canvas, int64_t delta_ms) override;

 public:
  float content_height() override;
  float content_width() override;

 public:
  std::string text;
  std::string text_file;
  skia::textlayout::TextStyle text_style;
  bool line_wrap = true;

 private:
  sk_sp<skia::textlayout::FontCollection> font_collection;
  skia::textlayout::ParagraphStyle paragraph_style;
  std::unique_ptr<skia::textlayout::ParagraphBuilder> paragraph_builder;
  std::unique_ptr<skia::textlayout::Paragraph> paragraph;

 private:
  bool refresh_layout = false;
  float layout_width_old = 0;

  skia::textlayout::PositionWithAffinity tlp_start;
  skia::textlayout::PositionWithAffinity tlp_end;
};

}  // namespace ikun_gui
