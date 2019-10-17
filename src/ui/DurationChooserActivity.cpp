/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#include "DurationChooserActivity.h"

DurationChooserActivity::DurationChooserActivity(String title,
                                                 uint32_t duration,
                                                 bool showTitleBar,
                                                 int titleFontScale,
                                                 bool showArrowHome)
    : Activity(title, showTitleBar, titleFontScale, showArrowHome)
    , duration(duration)
{
   // calculate hours, minutes and seconds
   hours = duration / 3600;
   duration = duration % 3600;
   minutes = duration / 60;
   duration = duration % 60;
   seconds = duration;
}

void DurationChooserActivity::drawLayout()
{
   Activity::drawLayout();

   uint8_t yMiddle = titleBarHeight + (display.height() - titleBarHeight) / 2;
   uint8_t xMiddle = display.width() / 2;
   uint8_t xThird = display.width() * 0.33333333f;

   // clear content
   display.fillRect(0, 30, display.width(), 30, BLACK);

   // draw seperators
   display.setTextColor(WHITE);
   display.setTextSize(1);
   display.setCursor(xThird + 4, yMiddle);
   display.print(":");
   display.setCursor(xThird * 2 - 4, yMiddle);
   display.print(":");

   switch (activeView)
   {
      case 0:
         // draw the first view highlighted (hours)
         display.setCursor(xThird - 22, yMiddle - 4);
         display.setTextSize(2);
         display.printf("%02d", hours);

         display.setCursor(xMiddle - 4, yMiddle);
         display.setTextSize(1);
         display.printf("%02d", minutes);

         display.setCursor(xThird * 2 + 6, yMiddle);
         display.setTextSize(1);
         display.printf("%02d", seconds);
         break;
      case 1:
         // draw the second view highlighted (minutes)
         display.setCursor(xThird - 14, yMiddle);
         display.setTextSize(1);
         display.printf("%02d", hours);

         display.setCursor(xMiddle - 9, yMiddle - 4);
         display.setTextSize(2);
         display.printf("%02d", minutes);

         display.setCursor(xThird * 2 + 6, yMiddle);
         display.setTextSize(1);
         display.printf("%02d", seconds);
         break;
      case 2:
         // draw the third view highlighted (seconds)
         display.setCursor(xThird - 14, yMiddle);
         display.setTextSize(1);
         display.printf("%02d", hours);

         display.setCursor(xMiddle - 4, yMiddle);
         display.setTextSize(1);
         display.printf("%02d", minutes);

         display.setCursor(xThird * 2 + 6, yMiddle - 4);
         display.setTextSize(2);
         display.printf("%02d", seconds);
         break;
   }

   display.display();
}

void DurationChooserActivity::onScroll(int distance)
{
   switch (activeView)
   {
      case 0:
         hours += distance;
         if (hours < 0)
         {
            hours = 24 + hours;
         }
         else if (hours >= 24 && minutes == 59 && seconds == 59)
         {
            hours = 23;
         }
         else if (hours >= 24)
         {
            hours = hours % 24;
         }
         break;
      case 1:
         minutes += distance;
         if (minutes < 0)
         {
            minutes = 60 + minutes;
            if (hours > 0)
            {
               hours--;
            }
         }
         else if (minutes >= 60 && hours == 23)
         {
            minutes = 59;
         }
         else if (minutes >= 60)
         {
            minutes = minutes % 60;
            if (hours < 23)
            {
               hours++;
            }
         }
         break;
      case 2:
         seconds += distance;
         if (seconds < 0)
         {
            seconds = 60 + seconds;
            if (minutes > 0)
            {
               minutes--;
            }
            else if (hours > 0 && minutes == 0)
            {
               hours--;
               minutes = 59;
            }
         }
         else if (seconds >= 60 && minutes == 59 && hours == 23)
         {
            seconds = 59;
         }
         else if (seconds >= 60)
         {
            seconds = seconds % 60;
            if (hours != 23 && minutes != 59 || hours == 23 && minutes < 59)
            {
               minutes++;
               if (minutes >= 60)
               {
                  minutes = minutes % 60;
                  if (hours > 23)
                  {
                     hours++;
                  }
               }
            }
         }
         break;
   }
   duration = hours * 3600 + minutes * 60 + seconds;
   drawLayout();
}

void DurationChooserActivity::onClick()
{
   // switch activeView to next view
   activeView++;
   if (activeView > 2)
   {
      activeView = 0;
   }
   drawLayout();
}

void DurationChooserActivity::setResult(ByteStack &bytes)
{
   // store duration in ByteStack bytes
   bytes.push(duration);
}

void DurationChooserActivity::onStart()
{
   Activity::onStart();
}

void DurationChooserActivity::onPause()
{
   Activity::onPause();
}

void DurationChooserActivity::onResume()
{
   Activity::onResume();
}

void DurationChooserActivity::onDestroy()
{
   Activity::onDestroy();
}