#include "App.h"

namespace ikun_gui {

std::shared_ptr<App> App::create() {
  auto app = std::make_shared<App>();
  app->vdom->set_runtime(app.get());
  return app;
}

void App::resize(int w, int h) {
  this->width = w;
  this->height = h;
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

std::shared_ptr<VNode> &App::root() {
  return vdom;
}

VNode *App::get_vdom() {
  return vdom.get();
}

VNode *App::get_waiting_event_node() {
  return waiting_event_node;
}

void App::set_waiting_event_node(VNode *node) {
  waiting_event_node = node;
}

void App::request_render() {
  if (request_render_waiting) return;
  request_render_waiting = true;
  request_render_impl(request_render_id++);
}

App *App::app() {
  return this;
}

void App::on_event(ikun_gui_app::Application::Event event, long value) {
  fmt::println("on_event: {}, {}", fmt::underlying(event), value);
  if (event == ikun_gui_app::Application::Event::RequestRender) {
    request_render_waiting = false;
  }
}

}  // namespace ikun_gui
