#pragma once

namespace ikun_gui {

class VNode;

class IRuntime {
 public:
  virtual VNode* get_vdom() = 0;
  virtual VNode* get_waiting_event_node() = 0;
  virtual void set_waiting_event_node(VNode* node) = 0;
};

}  // namespace ikun_gui
