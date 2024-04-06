#include "ikun_gui.h"

using namespace ikun_gui_app;
using namespace ikun_gui;

static void app_component(VNode* parent) {
  auto counter = hook::use_signal<int>([] {
    return 0;
  });

  fmt::println("app_component: parent: {}, count: {}", (void*)parent, *counter);
  {
    auto top = VNode::create();
    parent->add_child(top);
    top->attrs.height.percent(80);
    top->attrs.width.percent(100);
    top->attrs.color.rgb(0, 119, 182);
    top->attrs.color.argb(222, 0, 119, 182);
    top->attrs.justify(YGJustifyCenter);
    top->attrs.align.items(YGAlignCenter);
    top->init_attrs();
    {
      auto label = Label::create();
      top->add_child(label);
      label->font_size = 100;
      label->color.white();
      label->text = std::to_string(counter);
      label->init_attrs();
    }
  }

  if (*counter > 0) {
    auto group = VNode::create();
    parent->add_child(group);
    group->attrs.position(YGPositionTypeAbsolute);
    group->attrs.width.percent(100);
    group->attrs.height.percent(80);
    group->attrs.align.items(YGAlignCenter);
    group->attrs.justify(YGJustifySpaceAround);
    group->attrs.flex_direction(YGFlexDirectionRow);
    group->init_attrs();

    for (int i = 0; i < *counter; i++) {
      auto node = VNode::create();
      group->add_child(node);
      node->attrs.height.percent(100);
      node->attrs.width.percent(100.f / (float)*counter - 0.2f);
      node->init_attrs();
      node->add_child(app_component);
    }
  }

  {
    auto bottom = VNode::create();
    parent->add_child(bottom);
    bottom->attrs.height.percent(20);
    bottom->attrs.width.percent(100);
    bottom->attrs.color.white();
    bottom->attrs.color.argb(222, 222, 222, 222);
    bottom->attrs.justify(YGJustifyCenter);
    bottom->attrs.align.items(YGAlignCenter);
    bottom->attrs.flex_direction(YGFlexDirectionRow);
    bottom->init_attrs();

    // button: +
    {
      auto button = Button::create();
      bottom->add_child(button);
      button->attrs.width.percent(40);
      button->attrs.height.percent(40);
      button->init_attrs();
      button->on_click = [counter]() mutable {
        fmt::println("on_click: +");
        counter++;
      };
      {
        auto label = Label::create();
        button->add_child(label);
        label->font_size = 30;
        label->color.black();
        label->bold = true;
        label->text = "+";
        label->init_attrs();
      }
    }

    // span
    {
      auto span = VNode::create();
      bottom->add_child(span);
      span->attrs.width.percent(10);
      span->attrs.height.percent(0);
      span->init_attrs();
    }

    // button: -
    {
      auto button = Button::create();
      bottom->add_child(button);
      button->attrs.width.percent(40);
      button->attrs.height.percent(40);
      button->init_attrs();
      button->on_click = [counter]() mutable {
        fmt::println("on_click: -");
        counter--;
      };
      {
        auto label = Label::create();
        button->add_child(label);
        label->font_size = 30;
        label->color.black();
        label->bold = true;
        label->text = "-";
        label->init_attrs();
      }
    }
  }
}

int main(int argc, char* argv[]) {
  auto app = App::create();
  app->width = 400;
  app->height = 350;
  app->title = "component";
  app->add_component(app_component);
  return ikun_gui::run(argc, argv, app);
}
