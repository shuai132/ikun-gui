#include "ikun_gui.h"

using namespace ikun_gui_app;
using namespace ikun_gui;

static std::shared_ptr<App> create_app() {
  auto app = App::create();
  app->width = 640;
  app->height = 480;
  auto root = app->root();
  {
    auto node = VNode::create();
    node->attrs.height.percent(50);
    node->attrs.width.percent(100);
    node->attrs.color = 0xffff0000;  // red
    node->touchstart = [](const MotionEvent& event) {
      fmt::println("touchstart: {}", event);
    };
    node->touchmove = [](const MotionEvent& event) {
      fmt::println("touchmove: {}", event);
    };
    node->touchend = [](const MotionEvent& event) {
      fmt::println("touchend: {}", event);
    };
    node->touchcancel = [](const MotionEvent& event) {
      fmt::println("touchcancel: {}", event);
    };
    node->on_click = [] {
      fmt::println("on_click: +");
    };
    node->init_attrs();
    root->add_child(std::move(node));
  }
  {
    auto node = VNode::create();
    node->attrs.height.percent(50);
    node->attrs.width.percent(100);
    node->attrs.color = 0xff00ff00;  // green
    node->on_click = [] {
      fmt::println("on_click: -");
    };
    node->init_attrs();
    root->add_child(std::move(node));
  }
  return app;
}

int main(int argc, char* argv[]) {
  return ikun_gui::run(argc, argv, create_app());
}