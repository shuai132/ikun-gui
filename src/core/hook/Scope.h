#pragma once

#include <utility>
#include <vector>

#include "Signal.h"
#include "core/base/copyable.hpp"
#include "core/base/noncopyable.hpp"

namespace ikun_gui {

class VNode;

namespace hook {

template <typename R>
using HookFn = R (*)();

class HookHandle : ikun_gui::detail::copyable {
 public:
  explicit HookHandle(void* fn, SignalAny signal) : handle(fn), signal(std::move(signal)) {}

  template <typename R>
  R call() {
    ((HookFn<R>)(handle))();
  }

  void* handle = nullptr;
  SignalAny signal;
};

class Scope : ikun_gui::detail::noncopyable {
 public:
  explicit Scope(VNode* node);
  VNode* scope_root() const;

  void invalid() const;

  VNode* node;
  std::function<std::shared_ptr<VNode>()> child_builder;
  std::vector<HookHandle> hooks;
};

}  // namespace hook
}  // namespace ikun_gui
