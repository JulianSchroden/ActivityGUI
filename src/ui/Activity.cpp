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

void Activity::drawLayout(DrawMode drawMode)
{
   if (showTitleBar_)
   {
      drawTitleBar(title_, titleFontScale_, showArrowHome_, drawMode);
   }
}

void Activity::drawTitleBar(std::string title,
                            int fontScale,
                            bool showArrowHome,
                            DrawMode drawMode)
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

   // draw exclamation mark behind titleBar text, to idicate that the ui might
   // be unresponsive
   if (uiFreezeIndicator_ == UIFreezeIndicator::Visible)
   {
      display().print("!");
   }

   // transfer buffer to display.
   if (drawMode == DrawMode::TransferBuffer)
   {
      display().display();
   }
}

void Activity::clearActivity(const int color, DrawMode drawMode)
{
   // clear activity layout by drawing a rectangle above it
   display().fillRect(0,
                      titleBarHeight_,
                      display().width(),
                      display().height() - titleBarHeight_,
                      color);
   if (drawMode == DrawMode::TransferBuffer)
   {
      display().display();
   }
}

std::string Activity::title()
{
   return title_;
}

int Activity::titleBarHeight()
{
   return titleBarHeight_;
}

void Activity::setUIFreezeIndicator(UIFreezeIndicator uiFreezeIndicator)
{
   uiFreezeIndicator_ = uiFreezeIndicator;
}

}  // namespace ActivityGUI