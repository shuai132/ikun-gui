#pragma once

#include "app/Application.h"
#include "app/Window.h"
#include "ikun_gui.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkScalar.h"
#include "include/core/SkTypes.h"
#include "tools/skui/ModifierKey.h"

class SkSurface;
class SkTypeface;

namespace ikun_gui {

class IkunApp : public ikun_gui_app::Application, ikun_gui_app::Window::Layer, ikun_gui::detail::noncopyable {
 public:
  IkunApp(int argc, char** argv, std::shared_ptr<App> app, void* platformData = nullptr);
  ~IkunApp() override;

  void onEvent(Event event, long value) override;

  void onBackendCreated() override;
  void onPaint(SkSurface*) override;
  void onResize(int width, int height) override;
  bool onChar(SkUnichar c, skui::ModifierKey modifiers) override;
  bool onTouch(intptr_t owner, skui::InputState, float x, float y) override;
  bool onMouse(int x, int y, skui::InputState, skui::ModifierKey) override;
  bool onMouseWheel(float deltaY, float deltaX, int x, int y, skui::ModifierKey) override;
  // Platform-detected gesture events
  virtual bool onFling(skui::InputState state) {
    fmt::println("onFling:{} ", (int)state);
    return false;
  }
  virtual bool onPinch(skui::InputState state, float scale, float x, float y) {
    fmt::println("onPinch:{} {} {} {}", (int)state, scale, x, y);
    return false;
  }

 private:
  void updateTitle();
  bool onTouchOrMouse(intptr_t owner, skui::InputState state, float x, float y, skui::ModifierKey);

  ikun_gui_app::Window* fWindow;
  ikun_gui_app::Window::BackendType fBackendType = (ikun_gui_app::Window::BackendType)0;  // first hardware backend

  std::shared_ptr<ikun_gui::App> app;
};

int run(int argc, char** argv, std::shared_ptr<App> app);

}  // namespace ikun_gui
