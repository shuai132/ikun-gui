#include "IkunApp.h"

#include "include/core/SkFont.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkPoint.h"
#include "include/core/SkString.h"
#include "include/core/SkSurface.h"
#include "include/core/SkTileMode.h"
#include "include/effects/SkGradientShader.h"
#include "tools/window/DisplayParams.h"

using namespace ikun_gui_app;
using skwindow::DisplayParams;

namespace ikun_gui {

IkunApp::IkunApp(int argc, char** argv, std::shared_ptr<App> app, void* platformData)
#if defined(SK_GL)
    : fBackendType(Window::kNativeGL_BackendType)
#elif defined(SK_VULKAN)
    : fBackendType(Window::kVulkan_BackendType)
#else
    : fBackendType(Window::kRaster_BackendType)
#endif
{
  SkGraphics::Init();

  fWindow = Window::CreateNativeWindow(platformData);
  fWindow->setRequestedDisplayParams(DisplayParams());
  fWindow->setSize(app->width, app->height);
  {
    app->request_render_impl = [this](long id) {
      fWindow->inval();
      fWindow->sendUserEvent(Event::RequestRender, id);
    };
    auto scale = fWindow->getScaleFactor();
    app->resize(app->width * scale, app->height * scale);
    app->process_layout();
  }

  // register callbacks
  fWindow->pushLayer(this);

  fWindow->attach(fBackendType);

  fWindow->setTitle(app->title.c_str());

  this->app = std::move(app);
}

IkunApp::~IkunApp() {
  fWindow->detach();
  delete fWindow;
}

void IkunApp::updateTitle() {
  if (!fWindow) {
    return;
  }

  if (Window::kRaster_BackendType == fBackendType) {
    fmt::println("backend: Raster");
  } else {
#if defined(SK_GL)
    fmt::println("backend: OpenGL");
#elif defined(SK_VULKAN)
    fmt::println("backend: Vulkan");
#elif defined(SK_DAWN)
    fmt::println("backend: Dawn");
#else
    fmt::println("Unknown GPU backend");
#endif
  }
}

void IkunApp::onBackendCreated() {
  this->updateTitle();
  fWindow->show();
  fWindow->inval();
}

void IkunApp::onPaint(SkSurface* surface) {
  auto canvas = surface->getCanvas();
  app->init_canvas(canvas);
  app->render();
}

void IkunApp::onResize(int width, int height) {
  printf("onResize: %d, %d\n", width, height);
  app->resize(width, height);
  app->process_layout();
}

void IkunApp::onEvent(Event event, long value) {
  app->on_event(event, value);
}

bool IkunApp::onChar(SkUnichar c, skui::ModifierKey modifiers) {
  if (' ' == c) {
    if (Window::kRaster_BackendType == fBackendType) {
#if defined(SK_GL)
      fBackendType = Window::kNativeGL_BackendType;
#elif defined(SK_VULKAN)
      fBackendType = Window::kVulkan_BackendType;
#else
      SkDebugf("No GPU backend configured\n");
      return true;
#endif
    } else {
      fBackendType = Window::kRaster_BackendType;
    }
    fWindow->detach();
    fWindow->attach(fBackendType);
  }
  return true;
}

bool IkunApp::onTouch(intptr_t owner, skui::InputState state, float x, float y) {
  static skui::ModifierKey key;
  return onTouchOrMouse(owner, state, (float)x, (float)y, key);
}

bool IkunApp::onMouse(int x, int y, skui::InputState state, skui::ModifierKey key) {
  return onTouchOrMouse(0, state, (float)x, (float)y, key);
}

bool IkunApp::onTouchOrMouse(intptr_t owner, skui::InputState state, float x, float y, skui::ModifierKey) {
  // fmt::println("onTouch: {} {} {}", fmt::underlying(state), x, y);
  MotionEvent event;
  switch (state) {
    case skui::InputState::kDown:
      event.action = Action::DOWN;
      break;
    case skui::InputState::kUp:
      event.action = Action::UP;
      break;
    case skui::InputState::kMove:
      event.action = Action::MOVE;
      break;
    case skui::InputState::kRight:
    case skui::InputState::kLeft:
      event.action = Action::UNKNOWN;
      break;
  }
  event.x = x;
  event.y = y;
  app->send_event(event);
  return true;
}

}  // namespace ikun_gui

namespace ikun_gui {

int run(int argc, char** argv, std::shared_ptr<App> app) {
  return ikun_gui_app::run([&](void* platform_data) {
    return new ikun_gui::IkunApp(argc, argv, app, platform_data);
  });
}

}  // namespace ikun_gui
