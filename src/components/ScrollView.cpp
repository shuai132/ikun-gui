#include "ScrollView.h"

namespace ikun_gui {

ScrollView::ScrollView() {
//  attrs.height.percent(100);
//  attrs.justify(YGJustifyFlexStart);
//  attrs.width.percent(100);
}

std::shared_ptr<ScrollView> ScrollView::create() {
  return std::make_shared<ScrollView>();
}

void ScrollView::init_attrs() {
  VNode::init_attrs();

  on_wheel = [this](const WheelEvent &e) {
//    matrix_to_child.setTranslate(e.delta_x, e.delta_y);
//    invalidate();
//    return ;

//    fmt::println("wtf1: {} {} {}", scroll_pos_left, scroll_pos_top, e);
    scroll_pos_top += e.delta_y;
    scroll_pos_left += e.delta_x;

    matrix_to_child.setTranslate(scroll_pos_left, scroll_pos_top);
    invalidate();
    return

    scroll_pos_top += e.delta_y;
    if (scroll_pos_top > 0) {
      scroll_pos_top = 0;
    }
    if (scroll_pos_top + content_height() < layout_height) {
      scroll_pos_top = layout_height - content_height();
    }
    if (scroll_pos_top != scroll_pos_top_old) {
      scroll_pos_top_old = scroll_pos_top;
      invalidate();
    }

    scroll_pos_left += e.delta_x;
    if (scroll_pos_left > 0) {
      scroll_pos_left = 0;
    }
    if (scroll_pos_left + content_width() < layout_width) {
      scroll_pos_left = layout_width - content_width();
    }
    if (scroll_pos_left != scroll_pos_left_old) {
      scroll_pos_left_old = scroll_pos_left;
      invalidate();
    }

    fmt::println("wtf2: {} {}", scroll_pos_left, scroll_pos_top);
  };
}

void ScrollView::draw_self(SkCanvas *canvas, int64_t delta_ms) {
  VNode::draw_self(canvas, delta_ms);
//  canvas->resetMatrix();
//  canvas->translate(scroll_pos_left, scroll_pos_top);
}

float ScrollView::content_height() {
  return layout_height;
}

float ScrollView::content_width() {
  return layout_width;
}

}  // namespace ikun_gui
