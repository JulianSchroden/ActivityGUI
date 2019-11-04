/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#include "DurationChooserActivity.h"

namespace ActivityGUI
{
DurationChooserActivity::DurationChooserActivity(std::string title,
                                                 uint32_t duration,
                                                 bool showTitleBar,
                                                 int titleFontScale,
                                                 bool showArrowHome)
    : Activity(std::move(title), showTitleBar, titleFontScale, showArrowHome)
    , duration_(duration)
{
   // calculate hours, minutes and seconds
   hours_ = duration_ / 3600;
   duration_ = duration_ % 3600;
   minutes_ = duration_ / 60;
   duration_ = duration_ % 60;
   seconds_ = duration_;
}

void DurationChooserActivity::onScroll(int distance)
{
   switch (activeView_)
   {
      case 0:
         hours_ += distance;
         if (hours_ < 0)
         {
            hours_ = 24 + hours_;
         }
         else if (hours_ >= 24 && minutes_ == 59 && seconds_ == 59)
         {
            hours_ = 23;
         }
         else if (hours_ >= 24)
         {
            hours_ = hours_ % 24;
         }
         break;
      case 1:
         minutes_ += distance;
         if (minutes_ < 0)
         {
            minutes_ = 60 + minutes_;
            if (hours_ > 0)
            {
               --hours_;
            }
         }
         else if (minutes_ >= 60 && hours_ == 23)
         {
            minutes_ = 59;
         }
         else if (minutes_ >= 60)
         {
            minutes_ = minutes_ % 60;
            if (hours_ < 23)
            {
               ++hours_;
            }
         }
         break;
      case 2:
         seconds_ += distance;
         if (seconds_ < 0)
         {
            seconds_ = 60 + seconds_;
            if (minutes_ > 0)
            {
               --minutes_;
            }
            else if (hours_ > 0 && minutes_ == 0)
            {
               --hours_;
               minutes_ = 59;
            }
         }
         else if (seconds_ >= 60 && minutes_ == 59 && hours_ == 23)
         {
            seconds_ = 59;
         }
         else if (seconds_ >= 60)
         {
            seconds_ = seconds_ % 60;
            if ((hours_ != 23 && minutes_ != 59) || (hours_ == 23 && minutes_ < 59))
            {
               ++minutes_;
               if (minutes_ >= 60)
               {
                  minutes_ = minutes_ % 60;
                  if (hours_ > 23)
                  {
                     ++hours_;
                  }
               }
            }
         }
         break;
   }
   duration_ = hours_ * 3600 + minutes_ * 60 + seconds_;
   drawLayout();
}

void DurationChooserActivity::onClick()
{
   // switch activeView_ to next view
   ++activeView_;
   if (activeView_ > 2)
   {
      activeView_ = 0;
   }
   drawLayout();
}

void DurationChooserActivity::setResult(ByteStack &bytes)
{
   // store duration in ByteStack bytes
   bytes.push(duration_);
}

void DurationChooserActivity::drawLayout(DrawMode drawMode)
{
   Activity::drawLayout(DrawMode::BufferOnly);

   uint8_t yMiddle =
       titleBarHeight() + (display().height() - titleBarHeight()) / 2;
   uint8_t xMiddle = display().width() / 2;
   uint8_t xThird = display().width() * 0.33333333f;

   // clear content
   display().fillRect(0, 30, display().width(), 30, BLACK);

   // draw seperators
   display().setTextColor(WHITE);
   display().setTextSize(1);
   display().setCursor(xThird + 4, yMiddle);
   display().print(":");
   display().setCursor(xThird * 2 - 4, yMiddle);
   display().print(":");

   switch (activeView_)
   {
      case 0:
         // draw the first view highlighted (hours)
         display().setCursor(xThird - 22, yMiddle - 4);
         display().setTextSize(2);
         display().printf("%02d", hours_);

         display().setCursor(xMiddle - 4, yMiddle);
         display().setTextSize(1);
         display().printf("%02d", minutes_);

         display().setCursor(xThird * 2 + 6, yMiddle);
         display().setTextSize(1);
         display().printf("%02d", seconds_);
         break;
      case 1:
         // draw the second view highlighted (minutes)
         display().setCursor(xThird - 14, yMiddle);
         display().setTextSize(1);
         display().printf("%02d", hours_);

         display().setCursor(xMiddle - 9, yMiddle - 4);
         display().setTextSize(2);
         display().printf("%02d", minutes_);

         display().setCursor(xThird * 2 + 6, yMiddle);
         display().setTextSize(1);
         display().printf("%02d", seconds_);
         break;
      case 2:
         // draw the third view highlighted (seconds)
         display().setCursor(xThird - 14, yMiddle);
         display().setTextSize(1);
         display().printf("%02d", hours_);

         display().setCursor(xMiddle - 4, yMiddle);
         display().setTextSize(1);
         display().printf("%02d", minutes_);

         display().setCursor(xThird * 2 + 6, yMiddle - 4);
         display().setTextSize(2);
         display().printf("%02d", seconds_);
         break;
   }

   if (drawMode == DrawMode::TransferBuffer)
   {
      display().display();
   }
}

}  // namespace ActivityGUI