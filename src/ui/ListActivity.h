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
   void drawLayout() override;

   //!
   //! Helper method to draw the scroll indicator.
   //! The \a draw parameter indicates whether the display buffer should be
   //! transferred to the display
   //!
   void drawScrollIndicator(const boolean draw = true);

   //!
   //! Helper method to draw the selection indicator.
   //! Sets the lastSelectionIndicator to the passed \a index.
   //! The \a draw parameter indicates whether the display buffer should be
   //! transferred to the display
   //!
   void drawSelectionIndicator(const int index, const boolean draw = true);

protected:  // ToDo: should be private
   std::vector<std::string> listItems;
   int selectedItem = 0;  // index of the current selected item

private:
   int lastSelectedItem = 0;  // index of the last selected item

   int selectionIndicator = 0;  // current position of the selection indicator
   int lastSelectionIndicator = 0;  // last position of the selection indicator

   int maxIndex;          // maximum list index
   int visibleItemCount;  // number of list items, which fit on the screen

   // repeatedly used constants to draw the layout
   int listItemHeight = TextUtils::getTextBounds("T", 1).getHeight() + 5;
   const int scrollBarWidth = 7;
   const int indicatorHeight = 5;
   const int indicatorWidth = 3;
};
}  // namespace ActivityGUI

#endif
