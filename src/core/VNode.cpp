#include "VNode.h"

namespace ikun_gui {

std::shared_ptr<VNode> VNode::create() {
  auto node = std::make_shared<VNode>();
  return node;
}

void VNode::add_child(std::shared_ptr<VNode> child) {
  auto index = children.size();
  YGNodeInsertChild(yoga_node, child->yoga_node, index);

  child->parent = shared_from_this();
  children.push_back(std::move(child));
}

void VNode::init_attrs() {
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

void VNode::layout() {
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

void VNode::invalidate() {
  auto p = parent.lock();
  if (p) {
    p->invalidate();
  }
  invalidated = true;
}

bool VNode::dispatch_touch_event(const MotionEvent& event) {
  for (auto it = children.rbegin(); it != children.rend(); ++it) {
    auto ret = (*it)->dispatch_touch_event(event);
    if (ret) break;
  }

  if (!on_event && !on_click) return false;

  float left = YGNodeLayoutGetLeft(yoga_node);
  float top = YGNodeLayoutGetTop(yoga_node);
  float width = YGNodeLayoutGetWidth(yoga_node);
  float height = YGNodeLayoutGetHeight(yoga_node);
  if ((left <= event.x && event.x <= left + width) && (top <= event.y && event.y <= top + height)) {
    if (on_event) on_event(event);
    if (event.action == Action::UP) {
      if (on_click) on_click();
    }
    return true;
  }
  return false;
}

void VNode::draw_self(SkCanvas* canvas) {
  float left = YGNodeLayoutGetLeft(yoga_node);
  float top = YGNodeLayoutGetTop(yoga_node);
  float width = YGNodeLayoutGetWidth(yoga_node);
  float height = YGNodeLayoutGetHeight(yoga_node);

  SkPaint paint;
  paint.setColor(attrs.color);

  SkRect rect = SkRect::MakeXYWH(left, top, width, height);
  canvas->drawRect(rect, paint);
}

}  // namespace ikun_gui
