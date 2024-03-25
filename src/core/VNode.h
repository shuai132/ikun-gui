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
    auto_,
    percent,
    px,
  } type{};

  union Value {
    float px;
    float percent;
  } value{};
};

enum class AlignType {
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

struct Color {
  void argb(uint8_t a, uint8_t r, uint8_t g, uint8_t b) {
    is_set = true;
    value_ = SkColorSetARGB(a, r, g, b);
  }
  void rgb(uint8_t r, uint8_t g, uint8_t b) {
    is_set = true;
    value_ = SkColorSetRGB(r, g, b);
  }
  void black() {
    is_set = true;
    value_ = SK_ColorBLACK;
  }
  void white() {
    is_set = true;
    value_ = SK_ColorWHITE;
  }
  void gray() {
    is_set = true;
    value_ = SK_ColorGRAY;
  }
  void value(uint32_t v) {
    is_set = true;
    value_ = v;
  }
  uint32_t value() const {
    return value_;
  }
  bool is_set = false;

 private:
  uint32_t value_;
};

struct Attrs {
  std::string tag;
  AttrValue width;
  AttrValue height;
  std::string background;
  Align align;
  Color color;
  std::string shadow;
  std::string direction;
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
  std::weak_ptr<VNode> parent;
  std::vector<std::shared_ptr<VNode>> children;

  YGNodeRef yoga_node = YGNodeNew();

 private:
  bool invalidated = true;
  bool is_touching = false;
};

using div = VNode;
using rect = VNode;

}  // namespace ikun_gui
