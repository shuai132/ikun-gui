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

}  // namespace ikun_gui
