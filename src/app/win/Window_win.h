/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef Window_win_DEFINED
#define Window_win_DEFINED

#include <windows.h>

#include "../Window.h"

namespace ikun_gui_app {

class Window_win : public Window {
 public:
  Window_win() : Window() {}
  ~Window_win() override;

  bool init(HINSTANCE instance);

  void setTitle(const char*) override;
  void show() override;

  bool attach(BackendType) override;

  void onInval() override;

  void setRequestedDisplayParams(const DisplayParams&, bool allowReattach) override;

 private:
  void closeWindow();

  HINSTANCE fHInstance;
  HWND fHWnd;
  BackendType fBackend;
  bool fInitializedBackend = false;
};

}  // namespace ikun_gui_app

#endif
