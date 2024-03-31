#pragma once

#include <any>
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
  explicit HookHandle(void* fn, std::any signal) : handle(fn), signal(std::move(signal)) {}

  template <typename R>
  R call() {
    return ((HookFn<R>)(handle))();
  }

  template <typename R>
  void call() {
    ((HookFn<R>)(handle))();
  }

  template <typename T>
  const Signal<T>& as_signal() {
    return std::any_cast<const Signal<T>&>(signal);
  }

  void* handle = nullptr;
  std::any signal;
};

class Scope : ikun_gui::detail::noncopyable {
 public:
  explicit Scope(VNode* parent);
  VNode* scope_parent() const;
  const std::vector<VNode*>& scope_nodes() const;

  void invalid();

  VNode* parent;
  std::vector<VNode*> nodes;
  uint32_t nodes_pos_begin = 0;
  uint32_t nodes_pos_end = 0;
  bool is_invalid = false;
  std::function<void(VNode* parent)> child_builder;
  std::vector<HookHandle> hooks;
};

}  // namespace hook
}  // namespace ikun_gui
