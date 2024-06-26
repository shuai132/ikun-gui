#pragma once

#include "FontManager.h"
#include "IRuntime.h"
#include "VNode.h"
#include "app/Application.h"
#include "hook/hook.h"

namespace ikun_gui {

class App : public IRuntime {
 public:
  static std::shared_ptr<App> create();

 public:
  void resize(int width, int height);
  void process_hooks();
  void layout();
  void render();

  void push_event(MotionEvent event);
  void send_event(MotionEvent event);
  void push_event(WheelEvent event);
  void send_event(WheelEvent event);
  void process_events();

  void set_canvas(SkCanvas* c);

  void add_component(ComponentBuilder builder);

  VNode* get_vdom() override;

  VNode* get_waiting_event_node() override;

  void set_waiting_event_node(VNode* node) override;

  void request_render() override;

  App* app() override;

  void on_event(ikun_gui_app::Application::Event event, long value);

 public:
  int width = 0;
  int height = 0;
  std::string title;

  FontManager font_manager;

  std::function<void(long id)> request_render_impl;

 private:
  std::shared_ptr<VNode> vdom = VNode::create();
  std::vector<MotionEvent> motion_events;
  std::vector<WheelEvent> wheel_events;
  SkCanvas* canvas = nullptr;
  VNode* waiting_event_node = nullptr;
  long request_render_id = 0;
  bool request_render_waiting = false;
};

}  // namespace ikun_gui
