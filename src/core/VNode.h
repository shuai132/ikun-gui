#pragma once

#include <string>

#include "IRuntime.h"
#include "MotionEvent.h"
#include "hook/Scope.h"
#include "include/core/SkCanvas.h"
#include "yoga/Yoga.h"

namespace ikun_gui {

using ComponentBuilder = std::function<void(VNode*)>;

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

struct Position {
  void operator()(YGPositionType v) {
    is_set = true;
    value = v;
  }

  bool is_set = false;
  YGPositionType value;
};

struct Justify {
  void operator()(YGJustify v) {
    is_set = true;
    value = v;
  }

  bool is_set = false;
  YGJustify value;
};

struct FlexDirection {
  void operator()(YGFlexDirection v) {
    is_set = true;
    value = v;
  }

  bool is_set = false;
  YGFlexDirection value;
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

struct BorderRadius {
  void operator()(float x, float y) {
    is_set = true;
    this->x = x;
    this->y = y;
  }

  bool is_set = false;
  float x;
  float y;
};

struct Attrs {
  std::string tag;
  AttrValue width;
  AttrValue height;
  Align align;
  Position position;
  Justify justify;
  FlexDirection flex_direction;
  Color color;
  BorderRadius border_radius;
};

class VNode : public std::enable_shared_from_this<VNode>, ikun_gui::detail::noncopyable {
 public:
  static std::shared_ptr<VNode> create();
  VNode() = default;
  virtual ~VNode();

 public:
  void add_child(std::shared_ptr<VNode> child);

  void add_child(ComponentBuilder builder, std::shared_ptr<hook::Scope> scope = nullptr);

  virtual void init_attrs();

 public:
  void layout() const;

  bool process_scopes();

  virtual void draw(SkCanvas* canvas, int64_t delta_ms);

  virtual void draw_self(SkCanvas* canvas, int64_t delta_ms);

  void invalidate() const;

  bool dispatch_touch_event(const MotionEvent& event);

  bool dispatch_wheel_event(const WheelEvent& event);

  bool on_touch_event(const MotionEvent& event);

  bool is_enable_touch() const;

  bool on_wheel_event(const WheelEvent& event);

  bool is_enable_wheel() const;

 public:
  Attrs attrs;

 public:
  // common event
  std::function<void(const MotionEvent&)> touchstart;
  std::function<void(const MotionEvent&)> touchmove;
  std::function<void(const MotionEvent&)> touchend;
  std::function<void(const MotionEvent&)> touchcancel;
  // click event
  std::function<void()> on_click;

  std::function<void(const WheelEvent&)> on_wheel;

 public:
  VNode* parent = nullptr;
  std::vector<std::shared_ptr<VNode>> children;
  std::vector<std::shared_ptr<hook::Scope>> scopes;

  YGNodeRef yoga_node = YGNodeNew();

  float layout_left = 0;
  float layout_top = 0;
  float layout_width = 0;
  float layout_height = 0;

 private:
  bool is_touching = false;
};

}  // namespace ikun_gui
