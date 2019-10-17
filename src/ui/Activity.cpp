/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#include "Activity.h"

#include "../Runtime.h"
#include "TextUtils.h"


Activity::Activity(String title,
                   bool showTitleBar,
                   int titleFontScale,
                   bool showArrowHome)
    : display(Runtime::getDisplay())
    , title(title)
    , showTitleBar(showTitleBar)
    , titleFontScale(titleFontScale)
    , showArrowHome(showArrowHome)
{
}

Activity::~Activity() {}

void Activity::onScroll(int distance) {}

void Activity::onClick() {}

void Activity::onLongClick()
{
   // the default behaviour of onLongClick stops the current activity
   stopActivity();
}

void Activity::setResult(ByteStack &bytes) {}

void Activity::onActivityResult(ByteStack &result, int8_t key) {}

void Activity::onStart()
{
   display.clearDisplay();
   drawLayout();
}

void Activity::onPause() {}

void Activity::onResume()
{
   display.clearDisplay();
   drawLayout();
}

void Activity::onDestroy() {}

void Activity::startActivity(Activity *const activity)
{
   Runtime::startActivity(activity);
}

void Activity::startActivityForResult(Activity *const activity, int8_t key)
{
   Runtime::startActivityForResult(activity, key);
}

void Activity::stopActivity()
{
   Runtime::stopActivity();
}

void Activity::drawLayout()
{
   if (showTitleBar)
   {
      drawTitleBar(title, titleFontScale, showArrowHome);
   }
}

void Activity::drawTitleBar(String title,
                            int fontScale,
                            bool showArrowHome,
                            bool draw)
{
   // draw titleBar background
   Dimension dim = TextUtils::getTextBounds(title, fontScale);
   titleBarHeight = dim.getHeight() + 4;
   display.fillRect(0, 0, display.width(), dim.getHeight() + 4, WHITE);

   // print titleBar text
   display.setCursor((display.width() - dim.getWidth()) / 2, 3);
   display.setTextSize(fontScale);
   display.setTextColor(BLACK);
   display.print(title);

   // draw arrowHome triangle
   if (showArrowHome)
   {
      display.fillTriangle(4,
                           titleBarHeight / 2,
                           12,
                           (titleBarHeight / 2) - 4,
                           12,
                           (titleBarHeight / 2) + 4,
                           BLACK);
   }

   // draw exclamation mark behind titleBar text, when interrupts are disabled
   if (!areInterruptsEnbabled)
   {
      display.print("!");
   }

   // transfer buffer to display.
   if (draw)
   {
      display.display();
   }
}

void Activity::clearActivity(const int color, const boolean draw)
{
   // clear activity layout by drawing a rectangle above it
   display.fillRect(0,
                    titleBarHeight,
                    display.width(),
                    display.height() - titleBarHeight,
                    color);
   if (draw)
   {
      display.display();
   }
}
