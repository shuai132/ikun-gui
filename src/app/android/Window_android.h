/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef Window_android_DEFINED
#define Window_android_DEFINED

#include "../Window.h"
#include "android/surface_glue_android.h"

namespace ikun_gui_app {

class Window_android : public Window {
 public:
  Window_android() : Window() {}
  ~Window_android() override {}

  bool init(SkiaAndroidApp* skiaAndroidApp);
  void initDisplay(ANativeWindow* window);
  void onDisplayDestroyed();

  void setTitle(const char*) override;
  void show() override {}

  bool attach(BackendType) override;
  void onInval() override;
  void setUIState(const char* state) override;

  void paintIfNeeded();

  bool scaleContentToFit() const override {
    return true;
  }

 private:
  SkiaAndroidApp* fSkiaAndroidApp = nullptr;
  BackendType fBackendType;
};

}  // namespace ikun_gui_app

#endif
