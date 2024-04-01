#include "IRuntime.h"

namespace ikun_gui {

IRuntime *IRuntime::s_current_runtime;

IRuntime *IRuntime::current_runtime() {
  return s_current_runtime;
}

void IRuntime::set_current_runtime(IRuntime *runtime) {
  s_current_runtime = runtime;
}

}  // namespace ikun_gui
