#pragma once

#include "fmt/format.h"
#include "tools/skui/ModifierKey.h"

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

struct WheelEvent {
  Action action = UNKNOWN;
  float x = 0;
  float y = 0;
  float delta_y = 0;
  float delta_x = 0;
  skui::ModifierKey key{};
};

const char* format_as(const Action& action);

std::string format_as(const MotionEvent& e);

std::string format_as(const WheelEvent& e);

}  // namespace ikun_gui
