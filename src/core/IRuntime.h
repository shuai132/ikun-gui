#pragma once

#include "base/noncopyable.hpp"

namespace ikun_gui {

class VNode;
class App;

class IRuntime : ikun_gui::detail::noncopyable {
 public:
  virtual ~IRuntime() = default;
  virtual VNode* get_vdom() = 0;
  virtual VNode* get_waiting_event_node() = 0;
  virtual void set_waiting_event_node(VNode* node) = 0;
  virtual void request_render() = 0;
  virtual App* app() = 0;

 public:
  static IRuntime* current_runtime();
  static void set_current_runtime(IRuntime*);

 private:
  static IRuntime* s_current_runtime;
};

}  // namespace ikun_gui
