#include "ikun_gui.h"

using namespace ikun_gui_app;
using namespace ikun_gui;

static void app_component(VNode* parent) {
  auto tv = TextView::create();
  tv->text_file = "text/test.txt";
  tv->init_attrs();
  parent->add_child(tv);
}

int main(int argc, char* argv[]) {
  auto app = App::create();
  app->width = 800;
  app->height = 600;
  app->title = "textview";
  app->add_component(app_component);
  return ikun_gui::run(argc, argv, app);
}
