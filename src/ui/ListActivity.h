/**
 *  Copyright (c) 2017-2019 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 *  full license information.
 */

#ifndef _LISTACTIVITY_h
#define _LISTACTIVITY_h

#include <vector>

#include "Activity.h"
#include "TextUtils.h"


namespace ActivityGUI
{
class ListActivity : public Activity
{
public:
   //!
   //! Create ListActivity instance.
   //! The ListActivity can be used to create a scrollable lists containing the
   //! provided \a items.
   //!
   ListActivity(std::vector<std::string> items,
                std::string title,
                bool showTitleBar,
                int titleFontScale,
                bool showArrowHome = false);

   void onScroll(int distance) override;
   void onClick() override;

   //!
   //! onItemClick is called when an onClick action is performed on a
   //! list item. The \a index indicates the index of the clicked list item.
   //!
   virtual void onItemClick(int index);

   void onStart() override;

protected:
   void drawLayout(DrawMode drawMode = DrawMode::TransferBuffer) override;

   //!
   //! Helper method to draw the scroll indicator.
   //! The \a drawMode parameter indicates whether the display buffer should be
   //! transferred to the display.
   //!
   void drawScrollIndicator(DrawMode drawMode = DrawMode::TransferBuffer);

   //!
   //! Helper method to draw the selection indicator.
   //! Sets the lastSelectionIndicator to the passed \a index.
   //! The \a drawMode parameter indicates whether the display buffer should be
   //! transferred to the display.
   //!
   void drawSelectionIndicator(DrawMode drawMode = DrawMode::TransferBuffer);

private:
   std::vector<std::string> listItems_;
   int selectedItem_ = 0;      // index of the current selected item
   int lastSelectedItem_ = 0;  // index of the last selected item

   int selectionIndicator_ = 0;  // current position of the selection indicator
   int lastSelectionIndicator_ = 0;  // last position of the selection indicator

   int maxIndex_;          // maximum list index
   int visibleItemCount_;  // number of list items, which fit on the screen

   // repeatedly used constants to draw the layout
   int listItemHeight_ = TextUtils::getTextBounds("T", 1).getHeight() + 5;
   const int scrollBarWidth_ = 7;
   const int indicatorWidth_ = 3;
   const int indicatorHeight_ = 6;
};
}  // namespace ActivityGUI

#endif
