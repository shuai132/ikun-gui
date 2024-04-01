#pragma once

#include "base/noncopyable.hpp"
#include "include/core/SkTypeface.h"
#include "tools/fonts/FontToolUtils.h"

namespace ikun_gui {

class FontManager : ikun_gui::detail::noncopyable {
 public:
  FontManager();

 public:
  sk_sp<SkTypeface> typeface = nullptr;
};

}  // namespace ikun_gui
