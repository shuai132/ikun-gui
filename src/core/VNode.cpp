#include "VNode.h"

#include "App.h"

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

  child->parent = this;
  child->set_runtime(runtime);
  children.push_back(std::move(child));
}

void VNode::add_child(std::function<void(VNode*)> builder, std::shared_ptr<hook::Scope> scope_ptr) {
  if (scope_ptr == nullptr) {
    scope_ptr = std::make_shared<hook::Scope>(this);
  }
  auto scope = scope_ptr.get();
  this->scopes.push_back(std::move(scope_ptr));
  hook::set_current_scope(scope);
  scope->nodes_pos_begin = children.size();
  builder(this);
  scope->child_builder = std::move(builder);
  scope->nodes_pos_end = children.size();
  for (int i = 0; i < scope->nodes_pos_end - scope->nodes_pos_begin; i++) {
    scope->nodes.push_back(children[i + scope->nodes_pos_begin].get());
  }
}

void VNode::init_attrs() {
  // width
  switch (attrs.width.type) {
    case AttrValue::Type::unset:
      break;
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
    case AttrValue::Type::unset:
      break;
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

  // align
  switch (attrs.align.type) {
    case AlignType::unset:
      break;
    case AlignType::content:
      YGNodeStyleSetAlignContent(yoga_node, attrs.align.style);
      break;
    case AlignType::items:
      YGNodeStyleSetAlignItems(yoga_node, attrs.align.style);
      break;
    case AlignType::self:
      YGNodeStyleSetAlignSelf(yoga_node, attrs.align.style);
      break;
  }

  // position
  if (attrs.position.is_set) {
    YGNodeStyleSetPositionType(yoga_node, attrs.position.value);
  }

  // justify
  if (attrs.flex_direction.is_set) {
    YGNodeStyleSetJustifyContent(yoga_node, attrs.justify.value);
  }

  // flex_direction
  if (attrs.flex_direction.is_set) {
    YGNodeStyleSetFlexDirection(yoga_node, attrs.flex_direction.value);
  }
}

void VNode::layout() const {
  YGNodeCalculateLayout(yoga_node, YGUndefined, YGUndefined, YGDirectionLTR);
}

bool VNode::process_scopes() {
  auto has_any_invalid = std::any_of(this->scopes.begin(), this->scopes.end(), [&](const auto& item) {
    return item->is_invalid;
  });
  if (has_any_invalid) {
    // rebuild the node's child tree
    auto tmp_child = std::move(this->children);
    auto tmp_scopes = std::move(this->scopes);

    auto tmp_node_pos = tmp_child.cbegin();
    size_t tmp_node_index = 0;
    for (const auto& scope : tmp_scopes) {
      scope->is_invalid = false;
      uint32_t common_node_size = scope->nodes_pos_begin - tmp_node_index;
      if (common_node_size > 0) {
        this->children.assign(tmp_node_pos, tmp_node_pos + common_node_size);
        tmp_node_pos += common_node_size;
      }
      this->add_child(scope->child_builder, scope);
    }
    return true;
  }

  return std::any_of(children.begin(), children.end(), [&](const auto& item) {
    return item->process_scopes();
  });
}

void VNode::set_runtime(IRuntime* rt) {  // NOLINT(*-no-recursion)
  this->runtime = rt;
  for (const auto& item : children) {
    item->set_runtime(rt);
  }
}

void VNode::draw(SkCanvas* canvas, int64_t delta_ms) {  // NOLINT(*-no-recursion)
  if (parent != nullptr) {
    float left = YGNodeLayoutGetLeft(yoga_node);
    float top = YGNodeLayoutGetTop(yoga_node);
    layout_left = parent->layout_left + left;
    layout_top = parent->layout_top + top;
    layout_width = YGNodeLayoutGetWidth(yoga_node);
    layout_height = YGNodeLayoutGetHeight(yoga_node);
  }

  auto count = canvas->getSaveCount();
  draw_self(canvas, delta_ms);
  for (const auto& item : children) {
    item->draw(canvas, delta_ms);
  }
  canvas->restoreToCount(count);
}

void VNode::draw_self(SkCanvas* canvas, int64_t delta_ms) {
  if (!attrs.color.is_set) return;
  float width = YGNodeLayoutGetWidth(yoga_node);
  float height = YGNodeLayoutGetHeight(yoga_node);

  SkPaint paint;
  paint.setColor(attrs.color.value);

  SkRect rect = SkRect::MakeXYWH(layout_left, layout_top, width, height);
  canvas->drawRect(rect, paint);
}

void VNode::invalidate() {  // NOLINT(*-no-recursion)
  if (parent) {
    parent->invalidate();
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

  bool touch_is_on_node =
      (layout_left <= event.x && event.x <= layout_left + layout_width) && (layout_top <= event.y && event.y <= layout_top + layout_height);
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

VNode* VNode::root() const {
  return runtime->get_vdom();
}

bool VNode::is_root() const {
  return root() == this;
}

}  // namespace ikun_gui
