/*
 * Copyright 2019 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#import <Cocoa/Cocoa.h>

#include "../Application.h"
#include "Window_mac.h"

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>

@property(nonatomic, assign) BOOL done;

@end

@implementation AppDelegate : NSObject

@synthesize done = _done;

- (id)init {
  self = [super init];
  _done = FALSE;
  return self;
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)sender {
  _done = TRUE;
  return NSTerminateCancel;
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification {
  [NSApp stop:nil];
}

@end

///////////////////////////////////////////////////////////////////////////////////////////

using ikun_gui_app::Application;
using ikun_gui_app::Window_mac;

int ikun_gui_app::run(const create_app_proxy& proxy) {
#if MAC_OS_X_VERSION_MAX_ALLOWED < 1070
  // we only run on systems that support at least Core Profile 3.2
  return EXIT_FAILURE;
#endif

  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  [NSApplication sharedApplication];

  [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

  // Create the application menu.
  NSMenu* menuBar = [[NSMenu alloc] initWithTitle:@"AMainMenu"];
  [NSApp setMainMenu:menuBar];

  NSMenuItem* item;
  NSMenu* subMenu;

  item = [[NSMenuItem alloc] initWithTitle:@"Apple" action:nil keyEquivalent:@""];
  [menuBar addItem:item];
  subMenu = [[NSMenu alloc] initWithTitle:@"Apple"];
  [menuBar setSubmenu:subMenu forItem:item];
  [item release];
  item = [[NSMenuItem alloc] initWithTitle:@"Quit" action:@selector(terminate:) keyEquivalent:@"q"];
  [subMenu addItem:item];
  [item release];
  [subMenu release];

  // Set AppDelegate to catch certain global events
  AppDelegate* appDelegate = [[AppDelegate alloc] init];
  [NSApp setDelegate:appDelegate];

  Application* app = proxy(nullptr);

  // This will run until the application finishes launching, then lets us take over
  [NSApp run];

  // Now we process the events
  while (![appDelegate done]) {
    NSEvent* event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantFuture] inMode:NSDefaultRunLoopMode dequeue:YES];
    [NSApp sendEvent:event];

    if (event.type == NSEventTypeApplicationDefined) {
      auto e = (Application::Event)event.data1;
      if (Application::Event::RequestRender == e) {
        Window_mac::PaintWindows();
      }
      app->onEvent(e, event.data2);
    }
  }

  delete app;

  [NSApp setDelegate:nil];
  [appDelegate release];

  [menuBar release];
  [pool release];

  return EXIT_SUCCESS;
}
