#pragma once

#include "IRuntime.h"
#include "VNode.h"

namespace ikun_gui {

class App : public IRuntime {
 public:
  static std::shared_ptr<App> create();

 public:
  void resize(int width, int height);
  void process_layout();
  void render(VNode* node = nullptr);

  void push_event(MotionEvent event);
  void send_event(MotionEvent event);
  void process_events();

  void init_canvas(SkCanvas* c);

  std::shared_ptr<VNode>& root() {
    return vdom;
  }

  VNode* get_vdom() override {
    return vdom.get();
  }

  VNode* get_waiting_event_node() override {
    return waiting_event_node;
  }

  void set_waiting_event_node(VNode* node) override {
    waiting_event_node = node;
  }

 public:
  int width = 0;
  int height = 0;

 private:
  std::shared_ptr<VNode> vdom = VNode::create();
  std::vector<MotionEvent> events;
  SkCanvas* canvas = nullptr;
  VNode* waiting_event_node = nullptr;
};

}  // namespace ikun_gui
