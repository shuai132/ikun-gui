#pragma once

#include "core/VNode.h"
#include "modules/skparagraph/include/Paragraph.h"
#include "modules/skparagraph/include/ParagraphStyle.h"
#include "modules/skparagraph/src/ParagraphBuilderImpl.h"

namespace ikun_gui {

class TextView : public VNode {
 public:
  static std::shared_ptr<TextView> create();
  TextView();
  ~TextView() override = default;

  void init_attrs() override;

  void draw_self(SkCanvas* canvas, int64_t delta_ms) override;

 public:
  std::string text;
  std::string text_file;
  skia::textlayout::TextStyle text_style;

 private:
  sk_sp<skia::textlayout::FontCollection> font_collection;
  skia::textlayout::ParagraphStyle paragraph_style;
  std::unique_ptr<skia::textlayout::ParagraphBuilder> paragraph_builder;
  std::unique_ptr<skia::textlayout::Paragraph> paragraph;
};

}  // namespace ikun_gui
