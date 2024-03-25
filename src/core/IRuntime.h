#pragma once

namespace ikun_gui {

class VNode;
class App;

class IRuntime {
 public:
  virtual ~IRuntime() = default;
  virtual VNode* get_vdom() = 0;
  virtual VNode* get_waiting_event_node() = 0;
  virtual void set_waiting_event_node(VNode* node) = 0;
  virtual void request_render() = 0;
  virtual App* app() = 0;
};

}  // namespace ikun_gui
