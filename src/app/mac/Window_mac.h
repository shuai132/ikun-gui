/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef Window_mac_DEFINED
#define Window_mac_DEFINED

#import <Cocoa/Cocoa.h>

#include "../Window.h"
#include "src/core/SkTDynamicHash.h"

namespace ikun_gui_app {

class Window_mac : public Window {
 public:
  Window_mac() : Window(), fWindow(nil) {}
  ~Window_mac() override {
    this->closeWindow();
  }

  bool initWindow();

  double getScaleFactor() override;
  void setSize(int width, int height) override;
  void setTitle(const char*) override;
  void show() override;

  void sendUserEvent(Application::Event event, long value) override;

  bool attach(BackendType) override;

  void onInval() override {}

  float scaleFactor() const override;

  static void PaintWindows();

  static const NSInteger& GetKey(const Window_mac& w) {
    return w.fWindowNumber;
  }

  static uint32_t Hash(const NSInteger& windowNumber) {
    return windowNumber;
  }

  NSWindow* window() {
    return fWindow;
  }
  void closeWindow();

 private:
  NSWindow* fWindow;
  NSInteger fWindowNumber;

  static SkTDynamicHash<Window_mac, NSInteger> gWindowMap;
};

}  // namespace ikun_gui_app

#endif
