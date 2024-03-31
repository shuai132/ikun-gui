#pragma once

#include "Scope.h"
#include "Signal.h"

namespace ikun_gui {
namespace hook {

template <typename T>
Signal<T> use_signal(HookFn<T> f) {
  auto& hooks = hook::current_scope()->hooks;
  auto ret = std::find_if(hooks.begin(), hooks.end(), [&f](HookHandle& hook) {
    return f == hook.handle;
  });
  if (ret != hooks.cend()) {
    return ret->template as_signal<T>();
  } else {
    auto signal = Signal<T>(f());
    hooks.push_back(HookHandle{(void*)f, signal});
    return signal;
  }
}

}  // namespace hook
}  // namespace ikun_gui
