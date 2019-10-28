/**
 *  Copyright (c) 2017-2019 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 *  full license information.
 */

#ifndef _ACTIVITY_h
#define _ACTIVITY_h

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include <string>

#include "../utils/ByteStack.h"


namespace ActivityGUI
{
class Runtime;

class Activity
{
public:
   //!
   //! Create an Activity instance.
   //! The \a title can be displayed in the titlebar, when \a showTitleBar is
   //! set to true. The \a titleFontScale parameter can set the scale of the
   //! font and the \a showArrowHome parameter can be set to true, in case an
   //! arrow should be displayed in the titlebar to indicate, that the activity
   //! has a parent one.
   //!
   Activity(std::string title,
            bool showTitleBar = false,
            int titleFontScale = 2,
            bool showArrowHome = false);

   //!
   //! Activity destructor
   //!
   virtual ~Activity() = default;

   //!
   //! Set the \a runtime.
   //! Needs to be called before the \a onStart() function is called.
   //!
   void setRuntime(Runtime *runtime);

   //!
   //! Notify the activity, that the user scrolled a certain \a distance.
   //!
   virtual void onScroll(int distance);

   //!
   //! Notify the activity, that the user performed a click action.
   //!
   virtual void onClick();

   //!
   //! Notify the activity, that the user performed a longClick action.
   //!
   virtual void onLongClick();

   //!
   //! Callback which is called when the user navigates to the upper activity
   //! and a result is expected. The \a byteStack can be used to pass multiple
   //! primitive variables easily.
   //!
   virtual void setResult(ByteStack &byteStack);

   //!
   //! Callback which receives the result data of the child activity.
   //!
   virtual void onActivityResult(ByteStack result, uint8_t key);

   //!
   //! Runtime callback to start the activity.
   //!
   virtual void onStart();

   //!
   //! Runtime callback to pause the activity.
   //!
   virtual void onPause();

   //!
   //! Runtime callback to resume the acitivty.
   //!
   virtual void onResume();

   //!
   //! Runtime callback which is called before the Acitivity is destroyed.
   //!
   virtual void onDestroy();

protected:
   //!
   //! Get the runtime pointer.
   //!
   Runtime *runtime();

   //!
   //! Get the display pointer.
   //!
   Adafruit_SSD1306 &display();

protected:
   //!
   //!  Draw the activity layout.
   //!  Needs to be implemented by all child classes.
   //!
   virtual void drawLayout();

   //!
   //! Draw the titlebar to the top of the display.
   //! Draw the \a title with the provided \a fontScale to the titlebar.
   //! If \a showArrowHome is set to true, a small arrow is drawn on the
   //! titlebar to indicate that the activity has a parent one. The \a draw
   //! boolean indicates whether the display buffer should be transferred to
   //! the display.
   //!
   void drawTitleBar(std::string title,
                     int fontScale,
                     bool showArrowHome,
                     bool draw = false);
   
   //!
   //! Get the titleBarHeight
   //!
   int titleBarHeight();

   //!
   //! Clear the display
   //! The display is filled with the provided \a color (BLACK|WHITE) and the \a
   //! draw  boolean indicates whether the display buffer should be transferred
   //! to the display.
   //!
   void clearActivity(const int color = BLACK, boolean draw = true);

protected:  // ToDo: should be private
   std::string title_;
   bool showTitleBar_;
   int titleFontScale_;
   bool showArrowHome_;

   bool areInterruptsEnbabled_ = true;

private:
   int titleBarHeight_ = 0;
   Runtime *runtime_;
};
}  // namespace ActivityGUI

#endif
