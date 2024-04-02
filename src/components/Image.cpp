#include "Image.h"

#include "include/codec/SkCodec.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/encode/SkPngEncoder.h"
#include "modules/svg/include/SkSVGDOM.h"

namespace ikun_gui {

std::shared_ptr<Image> Image::create() {
  return std::make_shared<Image>();
}

void Image::init_attrs() {
  VNode::init_attrs();
  if (file_path.empty()) return;

  // todo: load on io thread
  auto is_img_file = [&] {
    // clang-format off
    return SkStrEndsWith(file_path.c_str(), ".png")
        || SkStrEndsWith(file_path.c_str(), ".jpg")
        || SkStrEndsWith(file_path.c_str(), ".jpeg");
    // clang-format on
  }();
  if (is_img_file) {
    sk_sp<SkData> data = SkData::MakeFromFileName(file_path.c_str());
    auto codec = SkCodec::MakeFromData(data);
    auto [image, result] = codec->getImage();
    sk_image = image;
  } else if (SkStrEndsWith(file_path.c_str(), ".svg")) {
    auto data = SkStream::MakeFromFile(file_path.c_str());
    sk_svg_dom = SkSVGDOM::MakeFromStream(*data);
    if (sk_svg_dom) {
      sk_svg_dom->setContainerSize(SkSize{layout_width, layout_height});
    }
  }
}

void Image::draw_self(SkCanvas *canvas, int64_t delta_ms) {
  if (sk_image) {
    auto rect = SkRect{layout_left, layout_top, layout_left + layout_width, layout_top + layout_height};
    canvas->drawImageRect(sk_image, rect, SkSamplingOptions{SkFilterMode::kNearest});
  } else if (sk_svg_dom) {
    canvas->save();
    canvas->translate(layout_left, layout_top);
    sk_svg_dom->render(canvas);
    canvas->restore();
  }
}

}  // namespace ikun_gui
