#include "App.h"

namespace ikun_gui {

std::shared_ptr<App> App::create() {
  auto app = std::make_shared<App>();
  app->vdom->runtime = app.get();
  return app;
}

void App::resize(int width, int height) {
  this->width = width;
  this->height = height;
  vdom->attrs.width.px(width);
  vdom->attrs.height.px(height);
  vdom->init_attrs();
}

void App::process_layout() {
  vdom->layout();
}

void App::render(VNode *node) {
  vdom->draw(canvas, 0);
}

void App::push_event(MotionEvent event) {
  events.push_back(std::move(event));
}

void App::send_event(MotionEvent event) {
  events.push_back(std::move(event));
  process_events();
}

void App::process_events() {
  for (const auto &e : events) {
    if (e.action != DOWN && waiting_event_node == nullptr) {
      // ignore event
      break;
    }
    if (waiting_event_node) {
      waiting_event_node->on_touch_event(e);
    } else {
      vdom->dispatch_touch_event(e);
    }
  }
  events.clear();
}

void App::init_canvas(SkCanvas *c) {
  canvas = c;
}

}  // namespace ikun_gui
