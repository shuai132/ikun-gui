#include "ikun_gui.h"

using namespace ikun_gui_app;
using namespace ikun_gui;

static void app_component(VNode* parent) {
  // ikun.jpeg
  {
    auto image = Image::create();
    image->attrs.width.percent(100);
    image->attrs.height.percent(50);
    image->file_path = "img/ikun.jpeg";
    image->init_attrs();
    parent->add_child(image);
  }

  // apple.svg
  {
    auto image = Image::create();
    image->attrs.width.percent(100);
    image->attrs.height.percent(50);
    image->file_path = "img/apple.svg";
    image->init_attrs();
    parent->add_child(image);
  }
}

int main(int argc, char* argv[]) {
  auto app = App::create();
  app->width = 504 / 2;
  app->height = 490;
  app->title = "image";
  app->add_component(app_component);
  return ikun_gui::run(argc, argv, app);
}
