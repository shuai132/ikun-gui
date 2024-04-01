#include "Scope.h"

#include "core/VNode.h"

namespace ikun_gui {
namespace hook {

Scope::Scope(VNode* parent) : parent(parent) {}

VNode* Scope::scope_parent() const {
  return parent;
}

const std::vector<VNode*>& Scope::scope_nodes() const {
  return nodes;
}

void Scope::invalid() {
  is_invalid = true;
  IRuntime::current_runtime()->request_render();
}

}  // namespace hook
}  // namespace ikun_gui
