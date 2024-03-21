#include "App.h"

namespace ikun_gui {

std::shared_ptr<App> App::create() {
  auto self = std::make_shared<App>();
  return self;
}

void App::resize(int width, int height) {
  this->width = width;
  this->height = height;
  vdom->attrs.width.px(width);
  vdom->attrs.height.px(height);
  vdom->init_attrs();
}

void App::process_layout() {
  vdom->layout(0, 0, 0, 0);
}

void App::render(VNode *node) {
  vdom->draw(canvas, 0);
}

void App::push_event(MotionEvent event) {
  events.push_back(std::move(event));
  process_events();
}

void App::process_events() {
  for (const auto &item : events) {
  }
}

void App::init_canvas(SkCanvas *c) {
  canvas = c;
}

}  // namespace ikun_gui
