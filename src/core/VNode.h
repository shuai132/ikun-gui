#pragma once

#include <string>

#include "IRuntime.h"
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
    unset,
    auto_,
    percent,
    px,
  } type = Type::unset;

  union Value {
    float px;
    float percent;
  } value{};
};

enum class AlignType {
  unset,
  content,
  items,
  self,
};

struct Align {
  void content(YGAlign align) {
    type = AlignType::content;
    style = align;
  }
  void items(YGAlign align) {
    type = AlignType::items;
    style = align;
  }

  void self(YGAlign align) {
    type = AlignType::self;
    style = align;
  }

  AlignType type;
  YGAlign style;
};

struct Justify {
  void operator()(YGJustify v) {
    is_set = true;
    justify = v;
  }

  bool is_set = false;
  YGJustify justify;
};

struct FlexDirection {
  void operator()(YGFlexDirection v) {
    is_set = true;
    direction = v;
  }

  bool is_set = false;
  YGFlexDirection direction;
};

struct Color {
  void operator()(SkColor v) {
    is_set = true;
    value = v;
  }

  void argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    is_set = true;
    value = SkColorSetARGB(a, r, g, b);
  }
  void rgb(uint8_t r, uint8_t g, uint8_t b) {
    is_set = true;
    value = SkColorSetRGB(r, g, b);
  }
  void black() {
    is_set = true;
    value = SK_ColorBLACK;
  }
  void white() {
    is_set = true;
    value = SK_ColorWHITE;
  }
  void gray() {
    is_set = true;
    value = SK_ColorGRAY;
  }

  bool is_set = false;
  uint32_t value = 0;
};

struct Attrs {
  std::string tag;
  AttrValue width;
  AttrValue height;
  std::string background;
  Align align;
  Justify justify;
  FlexDirection flex_direction;
  Color color;
  std::string shadow;
};

class VNode : public std::enable_shared_from_this<VNode> {
 public:
  static std::shared_ptr<VNode> create();
  VNode() = default;
  virtual ~VNode();

 public:
  void add_child(std::shared_ptr<VNode> child);

  virtual void init_attrs();

 public:
  void layout() const;

  void set_runtime(IRuntime* runtime);

  virtual void draw(SkCanvas* canvas, int64_t delta_ms);

  virtual void draw_self(SkCanvas* canvas, int64_t delta_ms);

  void invalidate();

  bool dispatch_touch_event(const MotionEvent& event);

  bool on_touch_event(const MotionEvent& event);

 public:
  Attrs attrs;

 public:
  // common event
  std::function<void(const MotionEvent&)> touchstart;
  std::function<void(const MotionEvent&)> touchmove;
  std::function<void(const MotionEvent&)> touchend;
  std::function<void(const MotionEvent&)> touchcancel;
  // control
  bool enable_touch = true;
  // click event
  std::function<void()> on_click;

 public:
  VNode* root() const;
  bool is_root() const;

 public:
  IRuntime* runtime = nullptr;
  VNode* parent = nullptr;
  std::vector<std::shared_ptr<VNode>> children;

  YGNodeRef yoga_node = YGNodeNew();

  float layout_left = 0;
  float layout_top = 0;
  float layout_width = 0;
  float layout_height = 0;

 private:
  bool invalidated = true;
  bool is_touching = false;
};

using div = VNode;
using rect = VNode;

}  // namespace ikun_gui
