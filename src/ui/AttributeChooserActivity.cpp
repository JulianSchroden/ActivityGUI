/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#include "AttributeChooserActivity.h"

namespace ActivityGUI
{
AttributeChooserActivity::AttributeChooserActivity(
    std::vector<AttributeChoice> &items,
    std::string title,
    uint8_t selected,
    bool showTitleBar,
    int titleFontScale,
    bool showArrowHome)
    : Activity(std::move(title), showTitleBar, titleFontScale, showArrowHome)
    , items(items)
    , selectedItem(selected)
{
}

void AttributeChooserActivity::drawLayout()
{
   Activity::drawLayout();

   // draw a triangle which points at the selected attribute
   int16_t yMiddle = titleBarHeight + (display.height() - titleBarHeight) / 2;
   display.fillTriangle(16, yMiddle - 5, 26, yMiddle, 16, yMiddle + 5, WHITE);

   std::vector<AttributeChoice>::iterator itemsIT = items.begin();
   // adavance list iterator to the new start-index
   std::advance(itemsIT, selectedItem - 1 >= 0 ? selectedItem - 1 : 0);

   // print the options
   display.setTextColor(WHITE);
   display.fillRect(36,
                    titleBarHeight,
                    display.width() - 36,
                    display.height() - titleBarHeight,
                    BLACK);
   for (int8_t i = 0; i < 4 && itemsIT != items.end(); i++)
   {
      if (i != 0 || i == 0 && selectedItem != 0)
      {  // skip the first position, when the first item is selected
         display.setCursor(36, yMiddle - 2 + (i - 1) * 14);
         display.print(itemsIT->getTitle().c_str());
         itemsIT++;
      }
   }

   display.display();
}

void AttributeChooserActivity::onScroll(int distance)
{
   selectedItem += distance;
   // jump to end, when user scrolls past the first element
   if (selectedItem < 0)
   {
      selectedItem = items.size() + selectedItem;
   }
   // jump to start, when user scrolls past the last element
   else if (selectedItem >= items.size())
   {
      selectedItem = selectedItem % items.size();
   }
   Serial.printf("selected = %d\n", selectedItem);
   // only draw the layout, when it has been changed
   if (selectedItem != lastSelectedItem)
   {
      drawLayout();
   }
   lastSelectedItem = selectedItem;
}

void AttributeChooserActivity::onClick() {}

void AttributeChooserActivity::onStart()
{
   Activity::onStart();
}

void AttributeChooserActivity::setResult(ByteStack &bytes)
{
   // store index of the selected item in the ByteStack
   bytes.push(selectedItem);
}

void AttributeChooserActivity::onPause()
{
   Activity::onPause();
}
void AttributeChooserActivity::onResume()
{
   Activity::onResume();
}
void AttributeChooserActivity::onDestroy()
{
   Activity::onDestroy();
}
}  // namespace ActivityGUI