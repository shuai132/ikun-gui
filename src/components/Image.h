#pragma once

#include "core/VNode.h"
#include "modules/svg/include/SkSVGDOM.h"

namespace ikun_gui {

class Image : public VNode {
 public:
  static std::shared_ptr<Image> create();
  Image() = default;
  ~Image() override = default;

  void init_attrs() override;

  void draw_self(SkCanvas* canvas, int64_t delta_ms) override;

 public:
  std::string file_path;
  sk_sp<SkImage> sk_image;
  sk_sp<SkSVGDOM> sk_svg_dom;
};

}  // namespace ikun_gui
