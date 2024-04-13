#include "ikun_gui.h"

using namespace ikun_gui_app;
using namespace ikun_gui;

static void app_component(VNode* parent) {
  parent->attrs.flex_direction(YGFlexDirectionRow);
  parent->init_attrs();
  {
    auto tv = TextView::create();
    tv->attrs.width.percent(50);
    tv->attrs.height.percent(100);
    tv->text_file = "text/test.txt";
    tv->init_attrs();
    parent->add_child(tv);
  }
  {
    auto span = VNode::create();
    span->attrs.width.px(1);
    span->attrs.height.percent(100);
    span->init_attrs();
    parent->add_child(span);
  }
  {
    auto tv = TextView::create();
    tv->attrs.width.percent(50);
    tv->attrs.height.percent(100);
    tv->text_file = "text/test.txt";
    tv->line_wrap = false;
    tv->init_attrs();
    parent->add_child(tv);
  }
}

int main(int argc, char* argv[]) {
  auto app = App::create();
  app->width = 800;
  app->height = 600;
  app->title = "textview";
  app->add_component(app_component);
  return ikun_gui::run(argc, argv, app);
}
