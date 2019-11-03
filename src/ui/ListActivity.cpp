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
    , listItems(std::move(items))
{
}

void ListActivity::onScroll(int distance)
{
   selectedItem += distance;
   // jump to end, when user scrolls past the first element
   if (selectedItem < 0)
   {
      selectedItem = listItems.size() + selectedItem;
   }
   // jump to start, when user scrolls past the last element
   else if (selectedItem >= listItems.size())
   {
      selectedItem = selectedItem % listItems.size();
   }

   LOG_D("Selected Item = %d", selectedItem);

   // only draw the layout, when it has been changed
   if (selectedItem != lastSelectedItem)
   {
      selectionIndicator = selectedItem;
      if (selectionIndicator > visibleItemCount - 1)
      {
         selectionIndicator = visibleItemCount - 1;
      }
      drawSelectionIndicator(selectionIndicator);
      drawScrollIndicator();
      // redraw list layout, when user scrolls past the last visible item
      if ((selectedItem > selectionIndicator) ||
          (selectedItem < visibleItemCount &&
           lastSelectedItem >= visibleItemCount))
      {
         drawLayout();
      }

      lastSelectedItem = selectedItem;
   }
}

void ListActivity::onClick()
{
   onItemClick(selectedItem);
}

void ListActivity::onItemClick(int index) {}

void ListActivity::onStart()
{
   // calculate the number of visible items
   visibleItemCount =
       ((display().height() - titleBarHeight()) / listItemHeight);
   maxIndex = listItems.size() - 1;

   Activity::onStart();
}

void ListActivity::drawLayout()
{
   Activity::drawLayout();

   auto newIT = listItems.begin();
   // adavance list iterator to new startIndex
   std::advance(newIT, selectedItem - selectionIndicator);

   display().setTextColor(WHITE);
   display().setTextSize(1);

   for (int i = 0; i <= visibleItemCount; i++)
   {
      // erase old text by drawing a black rectangle above it
      display().fillRect(listItemHeight + 4,
                         titleBarHeight() + i * listItemHeight + 3,
                         display().width() - listItemHeight - 4 - 8,
                         listItemHeight - 5,
                         BLACK);

      if (newIT != listItems.end())
      {
         // draw indicator background
         display().drawRect(4,
                            titleBarHeight() + i * listItemHeight + 2,
                            listItemHeight - 4,
                            listItemHeight - 4,
                            WHITE);
         // draw text
         display().setCursor(listItemHeight + 4,
                             titleBarHeight() + i * listItemHeight + 3);
         display().println(newIT->c_str());
         // Draw list seperator
         display().drawFastHLine(0,
                                 titleBarHeight() + (i + 1) * listItemHeight,
                                 display().width() - scrollBarWidth,
                                 WHITE);
         newIT++;
      }
   }
   // deleting the last indicator background, when the last item is selected
   if (selectedItem == maxIndex)
   {
      display().drawRect(
          4,
          titleBarHeight() + visibleItemCount * listItemHeight + 2,
          listItemHeight - 4,
          listItemHeight - 4,
          BLACK);
   }
   // redrawing the last indicator background, when the item before is selected
   else if (selectedItem == maxIndex - 1)
   {
      display().drawRect(
          4,
          titleBarHeight() + visibleItemCount * listItemHeight + 2,
          listItemHeight - 4,
          listItemHeight - 4,
          WHITE);
   }

   drawSelectionIndicator(selectionIndicator, false);
   drawScrollIndicator(false);
   display().display();  // lasts about 112ms
   //  1881 microseoconds to draw to buffer
   // 113645 micros to transfer buffer
}

void ListActivity::drawScrollIndicator(const boolean draw)
{
   // draw background
   display().fillRect(display().width() - scrollBarWidth,
                      titleBarHeight(),
                      scrollBarWidth,
                      display().height() - titleBarHeight(),
                      WHITE);

   // draw arrows
   display().fillTriangle(
       display().width() - scrollBarWidth + 1,
       titleBarHeight() + 3,
       display().width() - ceil((float)scrollBarWidth / 2.0f),
       titleBarHeight(),
       display().width() - 2,
       titleBarHeight() + 3,
       BLACK);
   display().fillTriangle(
       display().width() - scrollBarWidth + 1,
       display().height() - 4,
       display().width() - ceil((float)scrollBarWidth / 2.0f),
       display().height() - 2,
       display().width() - 2,
       display().height() - 4,
       BLACK);

   // draw Indicator
   int x = display().width() - ceil((float)scrollBarWidth / 2.0f) -
           (indicatorWidth / 2);
   int y = titleBarHeight() + 5 +
           ((float)selectedItem / (float)maxIndex) *
               (display().height() - titleBarHeight() - 11) -
           ((float)selectedItem / (float)maxIndex) * 5.0f;
   display().fillRect(x, y, 3, 6, BLACK);
   if (draw)
   {
      display().display();
   }
}

void ListActivity::drawSelectionIndicator(const int index, const boolean draw)
{
   // remove indicator of last selected item
   display().fillRect(
       6,
       titleBarHeight() + lastSelectionIndicator * listItemHeight + 4,
       listItemHeight - 8,
       listItemHeight - 8,
       BLACK);
   // draw indicator of the current selected item
   display().fillRect(6,
                      titleBarHeight() + index * listItemHeight + 4,
                      listItemHeight - 8,
                      listItemHeight - 8,
                      WHITE);
   if (draw)
   {
      display().display();
   }
   lastSelectionIndicator = index;
}

}  // namespace ActivityGUI