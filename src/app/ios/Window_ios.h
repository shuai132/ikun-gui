/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef Window_ios_DEFINED
#define Window_ios_DEFINED

#import <UIKit/UIKit.h>

#include "../Window.h"
#include "src/core/SkChecksum.h"
#include "src/core/SkTDynamicHash.h"

namespace ikun_gui_app {

class Window_ios : public Window {
 public:
  Window_ios() : Window(), fWindow(nil) {}
  ~Window_ios() override {
    this->closeWindow();
  }

  bool initWindow();

  void setTitle(const char*) override {}
  void show() override {}

  bool attach(BackendType) override;

  void onInval() override;

  static void PaintWindow();

  UIWindow* uiWindow() {
    return fWindow;
  }

  static Window_ios* MainWindow() {
    return gWindow;
  }

 private:
  void closeWindow();

  UIWindow* fWindow;

  static Window_ios* gWindow;  // there should be only one
};

}  // namespace ikun_gui_app

//////////////////////////////////////////////////////////////////////////

@interface MainView : UIView

- (MainView*)initWithWindow:(ikun_gui_app::Window_ios*)initWindow;

@end

#endif
