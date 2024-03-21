#pragma once

namespace ikun_gui {

enum Action {
  UNKNOWN = 0,
  DOWN = 1,
  MOVE = 2,
  UP = 3,
  CANCEL = 4,
  POINTER_DOWN = 5,
  POINTER_UP = 6,
};

struct MotionEvent {
  Action action = UNKNOWN;
  float x = 0;
  float y = 0;
};

}  // namespace ikun_gui
