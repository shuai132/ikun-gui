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

  enum class ObjectFit {
    FILL,     // 内容拉伸填充整个容器
    CONTAIN,  // 保持原有尺寸比例 最长边铺满容器
    COVER,    // 保持原有尺寸比例 最短边铺满容器
    NONE,     // 保持原有尺寸宽高 不缩放
  } object_fit = ObjectFit::CONTAIN;

  enum class Align {
    CENTER,  // 自动居中
    NONE,    // left-top
  } align = Align::CENTER;
};

}  // namespace ikun_gui
