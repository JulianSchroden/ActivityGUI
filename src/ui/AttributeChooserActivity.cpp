/**
 *  Copyright (c) 2017-2019 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 *  full license information.
 */

#include "AttributeChooserActivity.h"

#include "../utils/Logger.h"


namespace ActivityGUI
{
AttributeChooserActivity::AttributeChooserActivity(
    std::vector<AttributeChoice> items,
    std::string title,
    uint8_t selected,
    bool showTitleBar,
    int titleFontScale,
    bool showArrowHome)
    : Activity(std::move(title), showTitleBar, titleFontScale, showArrowHome)
    , items_(std::move(items))
    , selectedItem_(selected)
{
}

void AttributeChooserActivity::onScroll(int distance)
{
   selectedItem_ += distance;
   // jump to end, when user scrolls past the first element
   if (selectedItem_ < 0)
   {
      selectedItem_ = items_.size() + selectedItem_;
   }
   // jump to start, when user scrolls past the last element
   else if (selectedItem_ >= items_.size())
   {
      selectedItem_ = selectedItem_ % items_.size();
   }

   LOG_D("Selected item = %d", selectedItem_);

   // only draw the layout, when it has been changed
   if (selectedItem_ != lastSelectedItem_)
   {
      drawLayout();
   }
   lastSelectedItem_ = selectedItem_;
}

void AttributeChooserActivity::setResult(ByteStack &bytes)
{
   // store index of the selected item in the ByteStack
   bytes.push(selectedItem_);
}

void AttributeChooserActivity::drawLayout(DrawMode drawMode)
{
   Activity::drawLayout(DrawMode::BufferOnly);

   // draw a triangle which points at the selected attribute
   int16_t yMiddle =
       titleBarHeight() + (display().height() - titleBarHeight()) / 2;
   display().fillTriangle(16, yMiddle - 5, 26, yMiddle, 16, yMiddle + 5, WHITE);

   std::vector<AttributeChoice>::iterator itemsIT = items_.begin();
   // adavance list iterator to the new start-index
   std::advance(itemsIT, selectedItem_ - 1 >= 0 ? selectedItem_ - 1 : 0);

   // print the options
   display().setTextColor(WHITE);
   display().fillRect(36,
                      titleBarHeight(),
                      display().width() - 36,
                      display().height() - titleBarHeight(),
                      BLACK);
   for (int8_t i = 0; i < 4 && itemsIT != items_.end(); ++i)
   {
      if (i != 0 || i == 0 && selectedItem_ != 0)
      {  // skip the first position, when the first item is selected
         display().setCursor(36, yMiddle - 2 + (i - 1) * 14);
         display().print(itemsIT->title().c_str());
         ++itemsIT;
      }
   }

   if (drawMode == DrawMode::TransferBuffer)
   {
      display().display();
   }
}

}  // namespace ActivityGUI