#pragma once

#include "VNode.h"

namespace ikun_gui {

class App {
 public:
  static std::shared_ptr<App> create();

 public:
  void resize(int width, int height);
  void process_layout();
  void render(VNode* node = nullptr);

  void send_event(MotionEvent event);
  void process_events();

  void init_canvas(SkCanvas* c);

  std::shared_ptr<VNode>& root() {
    return vdom;
  }

 public:
  int width = 0;
  int height = 0;

 private:
  std::shared_ptr<VNode> vdom = VNode::create();
  std::vector<MotionEvent> events;
  SkCanvas* canvas = nullptr;
};

}  // namespace ikun_gui
