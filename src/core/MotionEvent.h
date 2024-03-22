#pragma once

#include "fmt/format.h"

namespace ikun_gui {

enum Action {
  UNKNOWN = 0,
  DOWN = 1,
  MOVE = 2,
  UP = 3,
  CANCEL = 4,
};

struct MotionEvent {
  Action action = UNKNOWN;
  float x = 0;
  float y = 0;
};

const char* format_as(const Action& action);

std::string format_as(const MotionEvent& f);

}  // namespace ikun_gui
