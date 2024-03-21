#pragma once

#include <string>

#include "MotionEvent.h"
#include "include/core/SkCanvas.h"
#include "yoga/Yoga.h"

namespace ikun_gui {

struct AttrValue {
  void percent(float percent) {
    type = Type::percent;
    value.percent = percent;
  }
  void px(float px) {
    type = Type::px;
    value.px = px;
  }
  void px(int px) {
    type = Type::px;
    value.px = (float)px;
  }
  void auto_() {
    type = Type::auto_;
  }
  enum class Type {
    auto_,
    percent,
    px,
  } type{};
  union Value {
    float px;
    float percent;
  } value{};
};

struct Attrs {
  std::string tag;
  AttrValue width;
  AttrValue height;
  std::string background;
  std::string main_align;
  std::string cross_align;
  uint32_t color = 0;
  std::string shadow;
  std::string direction;
};

class VNode : public std::enable_shared_from_this<VNode> {
 public:
  static std::shared_ptr<VNode> create();

  void add_child(std::shared_ptr<VNode> child);

  void init_attrs();

 public:
  void measure(uint32_t width, uint32_t height) {}

  void layout(int32_t l, int32_t t, int32_t r, int32_t b) {
    YGNodeCalculateLayout(yoga_node, YGUndefined, YGUndefined, YGDirectionLTR);
  }

  void draw(SkCanvas* canvas, int64_t delta_ms) {  // NOLINT(*-no-recursion)
    auto count = canvas->getSaveCount();
    draw_self(canvas);
    for (const auto& item : children) {
      item->draw(canvas, delta_ms);
    }
    canvas->restoreToCount(count);
  }

 private:
  void draw_self(SkCanvas* canvas) {
    float left = YGNodeLayoutGetLeft(yoga_node);
    float top = YGNodeLayoutGetTop(yoga_node);
    float width = YGNodeLayoutGetWidth(yoga_node);
    float height = YGNodeLayoutGetHeight(yoga_node);
    printf("draw_self: %f %f %f %f\n", left, top, width, height);

    SkPaint paint;
    paint.setColor(attrs.color);

    SkRect rect = SkRect::MakeXYWH(left, top, width, height);
    canvas->drawRect(rect, paint);

    printf("draw_self end\n");
  }

 public:
  virtual ~VNode() {
    YGNodeFree(yoga_node);
  }

 public:
  Attrs attrs;

 protected:
  std::weak_ptr<VNode> parent;
  std::vector<std::shared_ptr<VNode>> children;

  YGNodeRef yoga_node = YGNodeNew();
};

using div = VNode;
using rect = VNode;

}  // namespace ikun_gui
