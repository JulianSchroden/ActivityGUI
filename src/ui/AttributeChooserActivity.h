/**  
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.  
 *  Licensed under the MIT License. See LICENSE file in the project root for full license information.  
 */  

#ifndef _ATTRIBUTE_CHOOSER_ACTIVITY_h
#define _ATTRIBUTE_CHOOSER_ACTIVITY_h

#include "Activity.h"
#include "../utils/AttributeChoice.h"
#include <vector>


class AttributeChooserActivity: public Activity {
private:
    std::vector<AttributeChoice> &items;
    int8_t selectedItem     = 0; // index of the selected item
    int8_t lastSelectedItem = 0; // index of the last selected item

protected:    
    virtual void drawLayout();


public:
    /**
     *   AttributeChooserActivity constructor
     *   @param items           vector containing AttributeChoices 
     *   @param title            a string containing the activity-title which can be displayed in the titleBar
     *   @param selected         an integer index of the item, which is selected by default after launching the activity
	 *   @param showTitleBar     boolean value to choose, if the titleBar should be displayed
	 *   @param titleFontScale   an integer value > 0 to modify the size of the titleBar text; tested with the values 1 and 2 
	 *   @param showArrowHome    boolean value to choose if an arrow should indicate, that there is an upper activity
     */ 
    AttributeChooserActivity(std::vector<AttributeChoice> &items, String title, uint8_t selected=0, bool showTitleBar=false, int titleFontScale=2, bool showArrowHome=false);


    virtual void onScroll(int distance);
    virtual void onClick();


    virtual void onStart();
    virtual void onPause();
    virtual void onResume();
    virtual void onDestroy();

    virtual void setResult(ByteStack &bytes);
};


#endif