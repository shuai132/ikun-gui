#pragma once

#include <memory>
#include <string>

#include "Scope.h"
#include "core/base/copyable.hpp"
#include "core/base/noncopyable.hpp"
#include "scope_helper.h"

namespace ikun_gui {
namespace hook {

class SignalAny;

template <typename T>
class Signal {
 public:
  explicit Signal(T t) : v(std::make_shared<T>(t)) {}
  SignalAny erase_type() const;

  const T& get() const {
    return *v.get();
  }

  Signal<T>& operator++() {
    (*v.get())++;
    scope_invalid(scope);
    return *this;
  }

  Signal<T>& operator--() {
    (*v.get())--;
    scope_invalid(scope);
    return *this;
  }

  T operator++(T) {
    auto tmp = *v;
    ++(*this);
    return tmp;
  }

  T operator--(T) {
    auto tmp = *v;
    --(*this);
    return tmp;
  }

 private:
  std::shared_ptr<T> v;
  Scope* scope = hook::current_scope();
};

class SignalAny {
 public:
  template <typename T>
  Signal<T> as() const {
    return _place_holder;
  };

 private:
  Signal<int> _place_holder;
};

template <typename T>
SignalAny Signal<T>::erase_type() const {
  return *(SignalAny*)&v;
}

namespace detail {

template <typename T>
struct is_signal : std::false_type {};

template <typename... Args>
struct is_signal<Signal<Args...>> : std::true_type {};

}  // namespace detail

}  // namespace hook
}  // namespace ikun_gui

namespace std {

template <typename T, typename std::enable_if<ikun_gui::hook::detail::is_signal<T>::value, int>::type = 0>
inline std::string to_string(const T& t) {  // NOLINT(*-dcl58-cpp)
  return std::to_string(t.get());
}

}  // namespace std
