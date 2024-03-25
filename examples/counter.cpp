#include "ikun_gui.h"

using namespace ikun_gui_app;
using namespace ikun_gui;

static std::shared_ptr<App> create_app() {
  auto app = App::create();
  app->width = 400;
  app->height = 350;
  app->title = "counter";
  auto root = app->root();
  {
    auto node = VNode::create();
    node->attrs.height.percent(50);
    node->attrs.width.percent(100);
    node->attrs.color.rgb(0, 119, 182);
    node->attrs.align.self(YGAlign::YGAlignCenter);
    node->init_attrs();
    node->on_click = [] {
      fmt::println("on_click: +");
    };
    {
      auto label = Label::create();
      label->attrs.align.self(YGAlign::YGAlignCenter);
      label->font_size = 75;
      label->color = SK_ColorWHITE;
      label->text = "0";
      label->init_attrs();
      label->on_click = [] {
        fmt::println("on_click: label");
      };
      node->add_child(label);
    }
    root->add_child(std::move(node));
  }
  {
    auto node = VNode::create();
    node->attrs.height.percent(50);
    node->attrs.width.percent(100);
    node->attrs.color.white();
    node->on_click = [] {
      fmt::println("on_click: -");
    };
    node->init_attrs();
    root->add_child(std::move(node));
  }
  return app;
}

int main(int argc, char* argv[]) {
  return ikun_gui::run(argc, argv, create_app());
}
