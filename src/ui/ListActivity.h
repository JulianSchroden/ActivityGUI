/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#ifndef _LISTACTIVITY_h
#define _LISTACTIVITY_h

#include <list>

#include "Activity.h"
#include "TextUtils.h"

class ListActivity : public Activity
{
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

protected:
   std::list<String> &listItems;
   int selectedItem = 0;  // index of the current selected item

   virtual void drawLayout();

   /**
    *  Helper method which draws the scroll indicator
    *  @param draw   boolean value to choose if display buffer should be
    * transferred to display
    */
   void drawScrollIndicator(const boolean draw = true);

   /**
    *  Helper method to draw the selection indicator. Sets the
    * lastSelectionIndicator to the passed index
    *  @param index  index of the selected item
    *  @param draw   boolean value to choose if display buffer should be
    * transferred to display
    */
   void drawSelectionIndicator(const int index, const boolean draw = true);

public:
   /**
    *  ListActivity constructor
    *  @param items            a reference to a std::list containing the
    * strings, which should be displayed as list items
    *  @param title            a string containing the activity-title which can
    * be displayed in the titleBar
    *  @param showTitleBar     boolean value to choose, if the titleBar should
    * be displayed
    *  @param titleFontScale   an integer value > 0 to modify the size of the
    * titleBar text; tested with the values 1 and 2
    *  @param showArrowHome    boolean value to choose if an arrow should
    * indicate, that there is an upper activity
    */
   ListActivity(std::list<String> &items,
                String title,
                bool showTitleBar,
                int titleFontScale,
                bool showArrowHome = false);

   virtual void onScroll(int distance);
   virtual void onClick();

   /**
    *  virtual callback method, which is called by onClick and should be
    * implemented in the derived class
    *  @param index   the index of the list item which has been clicked
    */
   virtual void onItemClick(int index);

   virtual void onStart();
   virtual void onPause();
   virtual void onResume();
   virtual void onDestroy();
};

#endif
