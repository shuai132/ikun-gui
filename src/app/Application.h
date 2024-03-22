#pragma once

namespace ikun_gui_app {

class Application {
 public:
  enum class Event {
    RequestRender,
    UserEvent,
  };

 public:
  virtual ~Application() = default;

  virtual void onEvent(Event event, long value) = 0;
};

}  // namespace ikun_gui_app
