/**
 *  Copyright (c) 2017-2019 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 *  full license information.
 */

#include "Activity.h"

#include "../ActivityGUI.h"
#include "TextUtils.h"


namespace ActivityGUI
{
Activity::Activity(std::string title,
                   bool showTitleBar,
                   int titleFontScale,
                   bool showArrowHome)
    : title_(std::move(title))
    , showTitleBar_(showTitleBar)
    , titleFontScale_(titleFontScale)
    , showArrowHome_(showArrowHome)
{
}

void Activity::setRuntime(Runtime *runtime)
{
   runtime_ = runtime;
}

void Activity::onScroll(int distance) {}

void Activity::onClick() {}

void Activity::onLongClick()
{
   // the default behaviour of onLongClick stops the current activity
   runtime()->stopActivity();
}

void Activity::setResult(ByteStack &bytes) {}

void Activity::onActivityResult(ByteStack result, uint8_t key) {}

void Activity::onStart()
{
   display().clearDisplay();
   drawLayout();
}

void Activity::onPause() {}

void Activity::onResume()
{
   display().clearDisplay();
   drawLayout();
}

void Activity::onDestroy() {}

Runtime *Activity::runtime()
{
   return runtime_;
}

Adafruit_SSD1306 &Activity::display()
{
   return runtime_->display();
}

void Activity::drawLayout()
{
   if (showTitleBar_)
   {
      drawTitleBar(title_, titleFontScale_, showArrowHome_);
   }
}

void Activity::drawTitleBar(std::string title,
                            int fontScale,
                            bool showArrowHome,
                            bool draw)
{
   // draw titleBar background
   Dimension dim = TextUtils::getTextBounds(title, fontScale);
   titleBarHeight_ = dim.getHeight() + 4;
   display().fillRect(0, 0, display().width(), dim.getHeight() + 4, WHITE);

   // print titleBar text
   display().setCursor((display().width() - dim.getWidth()) / 2, 3);
   display().setTextSize(fontScale);
   display().setTextColor(BLACK);
   display().print(title.c_str());

   // draw arrowHome triangle
   if (showArrowHome)
   {
      display().fillTriangle(4,
                             titleBarHeight_ / 2,
                             12,
                             (titleBarHeight_ / 2) - 4,
                             12,
                             (titleBarHeight_ / 2) + 4,
                             BLACK);
   }

   // draw exclamation mark behind titleBar text, when interrupts are disabled
   if (!areInterruptsEnbabled_)
   {
      display().print("!");
   }

   // transfer buffer to display.
   if (draw)
   {
      display().display();
   }
}

void Activity::clearActivity(const int color, const boolean draw)
{
   // clear activity layout by drawing a rectangle above it
   display().fillRect(0,
                      titleBarHeight_,
                      display().width(),
                      display().height() - titleBarHeight_,
                      color);
   if (draw)
   {
      display().display();
   }
}
}  // namespace ActivityGUI