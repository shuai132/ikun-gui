#include "Image.h"

#include "include/codec/SkCodec.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/encode/SkPngEncoder.h"
#include "modules/svg/include/SkSVGDOM.h"
#include "modules/svg/include/SkSVGRenderContext.h"
#include "modules/svg/include/SkSVGSVG.h"

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
  VNode::draw_self(canvas, delta_ms);

  SkRect rect;
  SkSize size = [&] {
    if (sk_image) {
      return SkSize{(float)sk_image->width(), (float)sk_image->height()};
    } else if (sk_svg_dom) {
      SkSVGLengthContext length_context{SkSize{}};
      return sk_svg_dom->getRoot()->intrinsicSize(length_context);
    }
    return SkSize();
  }();
  if (size.isEmpty()) return;

  switch (object_fit) {
    case ObjectFit::FILL:
      rect = SkRect{0, 0, layout_width, layout_height};
      break;
    case ObjectFit::CONTAIN: {
      float scale_x = layout_width / size.width();
      float scale_y = layout_height / size.height();
      float scale = scale_x < scale_y ? scale_x : scale_y;
      rect = SkRect{0, 0, size.width() * scale, size.height() * scale};
    } break;
    case ObjectFit::COVER: {
      float scale_x = layout_width / size.width();
      float scale_y = layout_height / size.height();
      float scale = scale_x > scale_y ? scale_x : scale_y;
      rect = SkRect{0, 0, size.width() * scale, size.height() * scale};
    } break;
    case ObjectFit::NONE: {
      rect = SkRect{0, 0, size.width(), size.height()};
    } break;
  }

  switch (align) {
    case Align::CENTER:
      canvas->translate((layout_width - rect.width()) / 2, (layout_height - rect.height()) / 2);
      break;
    case Align::NONE:
      break;
  }

  if (sk_image) {
    canvas->drawImageRect(sk_image, rect, SkSamplingOptions{SkFilterMode::kNearest});
  } else if (sk_svg_dom) {
    float scale_x = rect.width() / size.width();
    float scale_y = rect.height() / size.height();
    canvas->scale(scale_x, scale_y);
    sk_svg_dom->render(canvas);
  }
}

}  // namespace ikun_gui
