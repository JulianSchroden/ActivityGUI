/**  
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.  
 *  Licensed under the MIT License. See LICENSE file in the project root for full license information.  
 */  

#ifndef _DURATION_CHOOSER_ACTIVITY_h
#define _DURATION_CHOOSER_ACTIVITY_h

#include "Activity.h"

class DurationChooserActivity:public Activity {

private:
    int8_t hours   = 0;
    int8_t minutes = 0;
    int8_t seconds = 0;

    uint32_t duration; // in seconds

    uint8_t activeView = 0; // index of the selected view

protected:
    virtual void drawLayout();

public:

    /**
     *   DurationChooserActivity constructor
     *   @param title            a string containing the activity-title which can be displayed in the titleBar
     *   @param duration         an integer value representing the default duration
	 *   @param showTitleBar     boolean value to choose, if the titleBar should be displayed
	 *   @param titleFontScale   an integer value > 0 to modify the size of the titleBar text; tested with the values 1 and 2 
	 *   @param showArrowHome    boolean value to choose if an arrow should indicate, that there is an upper activity
     */ 
    DurationChooserActivity(String title, uint32_t duration, bool showTitleBar=false, int titleFontScale=2, bool showArrowHome=false);

    virtual void onScroll(int distance);
	virtual void onClick();

	virtual void setResult(ByteStack &bytes);

    virtual void onStart();
	virtual void onPause();
	virtual void onResume();
	virtual void onDestroy();
     
};

#endif