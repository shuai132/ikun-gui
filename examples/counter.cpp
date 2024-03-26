#include "ikun_gui.h"

using namespace ikun_gui_app;
using namespace ikun_gui;

static std::shared_ptr<App> create_app() {
  auto app = App::create();
  app->width = 400;
  app->height = 350;
  app->title = "counter";

  std::shared_ptr<int> counter = std::make_shared<int>(0);

  auto root = app->root();
  {
    auto top = VNode::create();
    root->add_child(top);
    top->attrs.height.percent(50);
    top->attrs.width.percent(100);
    top->attrs.color.rgb(0, 119, 182);
    top->attrs.justify(YGJustifyCenter);
    top->attrs.align.items(YGAlignCenter);
    top->init_attrs();
    {
      auto label = Label::create();
      top->add_child(label);
      label->font_size = 100;
      label->color.white();
      label->text = std::to_string(*counter);
      label->init_attrs();
    }
  }

  {
    auto bottom = VNode::create();
    root->add_child(bottom);
    bottom->attrs.height.percent(50);
    bottom->attrs.width.percent(100);
    bottom->attrs.color.white();
    bottom->attrs.justify(YGJustifyCenter);
    bottom->attrs.align.items(YGAlignCenter);
    bottom->attrs.flex_direction(YGFlexDirectionRow);
    bottom->init_attrs();

    // button: +
    {
      auto button = Button::create();
      bottom->add_child(button);
      button->attrs.width.px(200);
      button->attrs.height.px(60);
      button->init_attrs();
      button->on_click = [counter] {
        fmt::println("on_click: +");
        (*counter)++;
      };
      {
        auto label = Label::create();
        button->add_child(label);
        label->font_size = 25;
        label->color.black();
        label->bold = true;
        label->text = "Increase";
        label->init_attrs();
      }
    }

    // span
    {
      auto span = VNode::create();
      bottom->add_child(span);
      span->attrs.width.percent(15);
      span->attrs.height.percent(0);
      span->init_attrs();
    }

    // button: -
    {
      auto button = Button::create();
      bottom->add_child(button);
      button->attrs.width.px(200);
      button->attrs.height.px(60);
      button->init_attrs();
      button->on_click = [counter] {
        fmt::println("on_click: -");
        (*counter)++;
      };
      {
        auto label = Label::create();
        button->add_child(label);
        label->font_size = 25;
        label->color.black();
        label->bold = true;
        label->text = "Decrease";
        label->enable_touch = false;
        label->init_attrs();
      }
    }
  }
  return app;
}

int main(int argc, char* argv[]) {
  return ikun_gui::run(argc, argv, create_app());
}
