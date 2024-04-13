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

std::string format_as(const MotionEvent& e) {
  return fmt::format("{}: x: {}, y: {}", e.action, e.x, e.y);
}

std::string format_as(const WheelEvent& e) {
  return fmt::format("{}: x: {}, y: {}, delta_y: {}, delta_x: {}, key: {}", e.action, e.x, e.y, e.delta_y, e.delta_x, fmt::underlying(e.key));
}

}  // namespace ikun_gui
