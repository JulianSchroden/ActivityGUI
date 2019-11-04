/**
 *  Copyright (c) 2017-2019 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 *  full license information.
 */

#include "ListActivity.h"

#include "../utils/Logger.h"


namespace ActivityGUI
{
ListActivity::ListActivity(std::vector<std::string> items,
                           std::string title,
                           bool showTitleBar,
                           int titleFontScale,
                           bool showArrowHome)
    : Activity(std::move(title), showTitleBar, titleFontScale, showArrowHome)
    , listItems_(std::move(items))
{
}

void ListActivity::onScroll(int distance)
{
   selectedItem_ += distance;
   // jump to end, when user scrolls past the first element
   if (selectedItem_ < 0)
   {
      selectedItem_ = listItems_.size() + selectedItem_;
   }
   // jump to start, when user scrolls past the last element
   else if (selectedItem_ >= listItems_.size())
   {
      selectedItem_ = selectedItem_ % listItems_.size();
   }

   LOG_D("Selected Item = %d", selectedItem_);

   drawLayout();

   lastSelectedItem_ = selectedItem_;
}

void ListActivity::onClick()
{
   onItemClick(selectedItem_);
}

void ListActivity::onItemClick(int index) {}

void ListActivity::onStart()
{
   // calculate the number of visible items
   visibleItemCount_ =
       ((display().height() - titleBarHeight()) / listItemHeight_);
   maxIndex_ = listItems_.size() - 1;

   Activity::onStart();
}

void ListActivity::drawLayout(DrawMode drawMode)
{
   Activity::drawLayout(DrawMode::BufferOnly);

   selectionIndicator_ = selectedItem_;
   if (selectionIndicator_ > visibleItemCount_ - 1)
   {
      selectionIndicator_ = visibleItemCount_ - 1;
   }

   auto newIT = listItems_.begin();
   // adavance list iterator to new startIndex
   std::advance(newIT, selectedItem_ - selectionIndicator_);

   display().setTextColor(WHITE);
   display().setTextSize(1);

   for (int i = 0; i <= visibleItemCount_; ++i)
   {
      // erase old text by drawing a black rectangle above it
      display().fillRect(listItemHeight_ + 4,
                         titleBarHeight() + i * listItemHeight_ + 3,
                         display().width() - listItemHeight_ - 4 - 8,
                         listItemHeight_ - 5,
                         BLACK);

      if (newIT != listItems_.end())
      {
         // draw indicator background
         display().drawRect(4,
                            titleBarHeight() + i * listItemHeight_ + 2,
                            listItemHeight_ - 4,
                            listItemHeight_ - 4,
                            WHITE);
         // draw text
         display().setCursor(listItemHeight_ + 4,
                             titleBarHeight() + i * listItemHeight_ + 3);
         display().println(newIT->c_str());
         // Draw list seperator
         display().drawFastHLine(0,
                                 titleBarHeight() + (i + 1) * listItemHeight_,
                                 display().width() - scrollBarWidth_,
                                 WHITE);
         ++newIT;
      }
   }
   // deleting the last indicator background, when the last item is selected
   if (selectedItem_ == maxIndex_)
   {
      display().drawRect(
          4,
          titleBarHeight() + visibleItemCount_ * listItemHeight_ + 2,
          listItemHeight_ - 4,
          listItemHeight_ - 4,
          BLACK);
   }
   // redrawing the last indicator background, when the item before is selected
   else if (selectedItem_ == maxIndex_ - 1)
   {
      display().drawRect(
          4,
          titleBarHeight() + visibleItemCount_ * listItemHeight_ + 2,
          listItemHeight_ - 4,
          listItemHeight_ - 4,
          WHITE);
   }

   drawselectionIndicator(DrawMode::BufferOnly);
   drawScrollIndicator(DrawMode::BufferOnly);
   if (drawMode == DrawMode::TransferBuffer)
   {
      display().display();
   }
}

void ListActivity::drawScrollIndicator(DrawMode drawMode)
{
   // draw background
   display().fillRect(display().width() - scrollBarWidth_,
                      titleBarHeight(),
                      scrollBarWidth_,
                      display().height() - titleBarHeight(),
                      WHITE);

   // draw arrows
   display().fillTriangle(
       display().width() - scrollBarWidth_ + 1,
       titleBarHeight() + 3,
       display().width() - ceil((float)scrollBarWidth_ / 2.0f),
       titleBarHeight(),
       display().width() - 2,
       titleBarHeight() + 3,
       BLACK);
   display().fillTriangle(
       display().width() - scrollBarWidth_ + 1,
       display().height() - 4,
       display().width() - ceil((float)scrollBarWidth_ / 2.0f),
       display().height() - 2,
       display().width() - 2,
       display().height() - 4,
       BLACK);

   // draw Indicator
   int x = display().width() - ceil((float)scrollBarWidth_ / 2.0f) -
           (indicatorWidth_ / 2);
   int y = titleBarHeight() + 5 +
           ((float)selectedItem_ / (float)maxIndex_) *
               (display().height() - titleBarHeight() - 11) -
           ((float)selectedItem_ / (float)maxIndex_) * 5.0f;
   display().fillRect(x, y, indicatorWidth_, indicatorHeight_, BLACK);
   if (drawMode == DrawMode::TransferBuffer)
   {
      display().display();
   }
}

void ListActivity::drawSelectionIndicator(DrawMode drawMode)
{
   // remove indicator of last selected item
   display().fillRect(
       6,
       titleBarHeight() + lastselectionIndicator_ * listItemHeight_ + 4,
       listItemHeight_ - 8,
       listItemHeight_ - 8,
       BLACK);
   // draw indicator of the current selected item
   display().fillRect(
       6,
       titleBarHeight() + selectionIndicator_ * listItemHeight_ + 4,
       listItemHeight_ - 8,
       listItemHeight_ - 8,
       WHITE);
   if (drawMode == DrawMode::TransferBuffer)
   {
      display().display();
   }
   lastselectionIndicator_ = selectionIndicator_;
}

}  // namespace ActivityGUI