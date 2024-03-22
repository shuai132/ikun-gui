#include "VNode.h"

namespace ikun_gui {

std::shared_ptr<VNode> VNode::create() {
  return std::make_shared<VNode>();
}

VNode::~VNode() {
  YGNodeFree(yoga_node);
}

void VNode::add_child(std::shared_ptr<VNode> child) {
  auto index = children.size();
  YGNodeInsertChild(yoga_node, child->yoga_node, index);

  child->parent = shared_from_this();
  child->runtime = runtime;
  children.push_back(std::move(child));
}

void VNode::init_attrs() const {
  // width
  switch (attrs.width.type) {
    case AttrValue::Type::px:
      YGNodeStyleSetWidth(yoga_node, attrs.width.value.px);
      break;
    case AttrValue::Type::percent:
      YGNodeStyleSetWidthPercent(yoga_node, attrs.width.value.percent);
      break;
    case AttrValue::Type::auto_:
      YGNodeStyleSetWidthAuto(yoga_node);
      break;
  }

  // height
  switch (attrs.height.type) {
    case AttrValue::Type::px:
      YGNodeStyleSetHeight(yoga_node, attrs.height.value.px);
      break;
    case AttrValue::Type::percent:
      YGNodeStyleSetHeightPercent(yoga_node, attrs.height.value.percent);
      break;
    case AttrValue::Type::auto_:
      YGNodeStyleSetWidthAuto(yoga_node);
      break;
  }
}

void VNode::layout() const {
  YGNodeCalculateLayout(yoga_node, YGUndefined, YGUndefined, YGDirectionLTR);
}

void VNode::draw(SkCanvas* canvas, int64_t delta_ms) {  // NOLINT(*-no-recursion)
  auto count = canvas->getSaveCount();
  draw_self(canvas);
  for (const auto& item : children) {
    item->draw(canvas, delta_ms);
  }
  canvas->restoreToCount(count);
}

void VNode::invalidate() {  // NOLINT(*-no-recursion)
  auto p = parent.lock();
  if (p) {
    p->invalidate();
  }
  invalidated = true;
}

bool VNode::dispatch_touch_event(const MotionEvent& event) {  // NOLINT(*-no-recursion)
  for (auto it = children.rbegin(); it != children.rend(); ++it) {
    auto ret = (*it)->dispatch_touch_event(event);
    if (ret) return true;
  }
  return on_touch_event(event);
}

bool VNode::on_touch_event(const MotionEvent& event) {
  if (!enable_touch) return false;

  float left = YGNodeLayoutGetLeft(yoga_node);
  float top = YGNodeLayoutGetTop(yoga_node);
  float width = YGNodeLayoutGetWidth(yoga_node);
  float height = YGNodeLayoutGetHeight(yoga_node);
  bool touch_is_on_node = (left <= event.x && event.x <= left + width) && (top <= event.y && event.y <= top + height);
  if (!is_touching) {
    if (touch_is_on_node && (event.action == Action::DOWN)) {
      is_touching = true;
      runtime->set_waiting_event_node(this);
      if (touchstart) touchstart(event);
      return true;
    } else {
      // should ignore
      return false;
    }
  } else {
    if (!touch_is_on_node) {
      is_touching = false;
      runtime->set_waiting_event_node(nullptr);
      if (touchcancel) touchcancel(event);
      return true;
    } else {
      if (event.action == Action::MOVE) {
        if (touchmove) touchmove(event);
        return true;
      } else if (event.action == Action::UP) {
        is_touching = false;
        runtime->set_waiting_event_node(nullptr);
        if (touchend) touchend(event);
        if (on_click) on_click();
        return true;
      }
    }
  }
  return false;
}

void VNode::draw_self(SkCanvas* canvas) const {
  float left = YGNodeLayoutGetLeft(yoga_node);
  float top = YGNodeLayoutGetTop(yoga_node);
  float width = YGNodeLayoutGetWidth(yoga_node);
  float height = YGNodeLayoutGetHeight(yoga_node);

  SkPaint paint;
  paint.setColor(attrs.color);

  SkRect rect = SkRect::MakeXYWH(left, top, width, height);
  canvas->drawRect(rect, paint);
}

VNode* VNode::root() const {
  return runtime->get_vdom();
}

bool VNode::is_root() const {
  return root() == this;
}

}  // namespace ikun_gui
