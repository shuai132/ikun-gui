#pragma once

namespace ikun_gui {
namespace hook {

class Scope;

void set_current_scope(Scope* scope);

Scope* current_scope();

void scope_invalid(Scope* scope);

}  // namespace hook
}  // namespace ikun_gui
