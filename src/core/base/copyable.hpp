#pragma once

namespace ikun_gui {
namespace detail {

class copyable {
 public:
  copyable(const copyable&) = default;
  copyable& operator=(const copyable&) = default;

 protected:
  copyable() = default;
  ~copyable() = default;
};

}  // namespace detail
}  // namespace ikun_gui
