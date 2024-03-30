#include "Scope.h"

#include "core/VNode.h"

namespace ikun_gui {
namespace hook {

Scope::Scope(VNode* node) : node(node) {}

VNode* Scope::scope_root() const {
  return node;
}

void Scope::invalid() const {
  node->children.clear();
  this->child_builder();
  node->runtime->request_render();
}

}  // namespace hook
}  // namespace ikun_gui
