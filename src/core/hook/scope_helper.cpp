#include "scope_helper.h"

#include "Scope.h"

namespace ikun_gui {
namespace hook {

static Scope* s_scope;

void set_current_scope(Scope* scope) {
  s_scope = scope;
}

Scope* current_scope() {
  return s_scope;
}

void scope_invalid(Scope* scope) {
  scope->invalid();
}

}  // namespace hook
}  // namespace ikun_gui
