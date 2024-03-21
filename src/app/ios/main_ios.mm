/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>

#include "Application.h"
#include "include/core/SkTypes.h"
#include "ios/Window_ios.h"

#if __has_feature(objc_arc)
#error "File should not be compiled with ARC."
#endif

namespace {

static int gArgc;
static char **gArgv;

}  // namespace

////////////////////////////////////////////////////////////////////

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@end

@implementation AppDelegate {
  CADisplayLink *fDisplayLink;  // Owned by the run loop.
  std::unique_ptr<ikun_gui_app::Application> fApp;
}

#pragma mark - UIApplicationDelegate

- (void)applicationWillResignActive:(UIApplication *)sender {
  ikun_gui_app::Window_ios *mainWindow = ikun_gui_app::Window_ios::MainWindow();
  if (mainWindow) {
    mainWindow->onActivate(false);
  }
}

- (void)applicationDidBecomeActive:(UIApplication *)sender {
  ikun_gui_app::Window_ios *mainWindow = ikun_gui_app::Window_ios::MainWindow();
  if (mainWindow) {
    mainWindow->onActivate(true);
  }
}

- (void)applicationWillTerminate:(UIApplication *)sender {
  // Display link retains us, so we break the cycle now.
  // Note: dealloc is never called.
  [fDisplayLink invalidate];
  fDisplayLink = nil;
  fApp.reset();
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  fApp = std::unique_ptr<ikun_gui_app::Application>(ikun_gui_app::Application::Create(gArgc, gArgv, nullptr));

  auto mainWindow = ikun_gui_app::Window_ios::MainWindow();
  mainWindow->onActivate(application.applicationState == UIApplicationStateActive);

  fDisplayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(displayLinkFired)];
  [fDisplayLink addToRunLoop:NSRunLoop.mainRunLoop forMode:NSRunLoopCommonModes];

  return YES;
}

- (void)displayLinkFired {
  // TODO: Hook into CAMetalLayer's drawing event loop or our own run loop observer.
  // Need to handle animated slides/redraw mode, so we need something that will wake up the
  // run loop.
  ikun_gui_app::Window_ios::PaintWindow();

  fApp->onIdle();
}

@end

///////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
  gArgc = argc;
  gArgv = argv;
  return UIApplicationMain(argc, argv, nil, @"AppDelegate");
}
