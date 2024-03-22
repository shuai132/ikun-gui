#include "MotionEvent.h"

namespace ikun_gui {

const char* format_as(const Action& action) {
  switch (action) {
    case ikun_gui::Action::UNKNOWN:
      return "UNKNOWN";
    case ikun_gui::Action::DOWN:
      return "DOWN";
    case ikun_gui::Action::MOVE:
      return "MOVE";
    case ikun_gui::Action::UP:
      return "UP";
    case ikun_gui::Action::CANCEL:
      return "CANCEL";
    default:
      return "ERROR";
  }
}

std::string format_as(const MotionEvent& f) {
  return fmt::format("{}: x: {}, y: {}", f.action, f.x, f.y);
}

}  // namespace ikun_gui
