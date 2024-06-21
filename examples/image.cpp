#include "ikun_gui.h"

using namespace ikun_gui_app;
using namespace ikun_gui;

static void app_component(VNode* parent) {
  //  parent->attrs.flex_direction(YGFlexDirectionRow);
  //  parent->init_attrs();

  auto scroll = ScrollView::create();
  scroll->init_attrs();
  parent->add_child(scroll);

  const auto box_px = 200;
  for (int i = 0; i < 1; ++i) {
    // ikun.jpeg
    {
      auto image = Image::create();
      image->attrs.width.px(box_px);
      image->attrs.height.px(box_px);
      image->file_path = "img/ikun.jpeg";
      image->init_attrs();
      image->on_click = [i, image] {
        fmt::println("ikun: i={} x={}, y={}", i, image->matrix_on_node.getTranslateX(), image->matrix_on_node.getTranslateY());
      };
      scroll->add_child(image);
    }

//    // apple.svg
//    {
//      auto image = Image::create();
//      image->attrs.width.px(box_px);
//      image->attrs.height.px(box_px);
//      image->attrs.color.gray();
//      image->file_path = "img/apple.svg";
//      image->init_attrs();
//      image->on_click = [i] {
//        fmt::println("apple: {}", i);
//      };
//      scroll->add_child(image);
//    }
  }
}

int main(int argc, char* argv[]) {
  auto app = App::create();
  app->width = 504;
  app->height = 490 / 2;
  app->title = "image";
  app->add_component(app_component);
  return ikun_gui::run(argc, argv, app);
}
