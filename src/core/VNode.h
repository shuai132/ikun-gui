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
  VNode() = default;
  virtual ~VNode();

 public:
  void add_child(std::shared_ptr<VNode> child);

  void init_attrs() const;

 public:
  void layout() const;

  void draw(SkCanvas* canvas, int64_t delta_ms);

  void invalidate();

  bool dispatch_touch_event(const MotionEvent& event);

  bool on_touch_event(const MotionEvent& event);

 private:
  void draw_self(SkCanvas* canvas) const;

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
