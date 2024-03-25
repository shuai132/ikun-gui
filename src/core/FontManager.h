#pragma once

#include "include/core/SkTypeface.h"
#include "tools/fonts/FontToolUtils.h"

namespace ikun_gui {

class FontManager {
 public:
  FontManager();

 public:
  sk_sp<SkTypeface> typeface = nullptr;
};

}  // namespace ikun_gui
