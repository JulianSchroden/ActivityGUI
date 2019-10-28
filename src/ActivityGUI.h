/**
 *  Copyright (c) 2017-2019 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 *  full license information.
 */

#ifndef _ACTIVITY_GUI_h
#define _ACTIVITY_GUI_h

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include <list>
#include <memory>
#include <stack>

#include "ActivityExecution.h"
#include "Worker.h"
#include "input/InputModule.h"
#include "ui/Activity.h"
#include "utils/ByteStack.h"


namespace ActivityGUI
{
class Runtime
{
public:
   //!
   //! Create a Runtime instance.
   //!
   Runtime(std::unique_ptr<InputModule> inputModule, Adafruit_SSD1306 display);

   //!
   //! Perform the Runtime's work
   //! This function needs to be called periodically.
   //!
   void runOnce();

   //!
   //! Push the provided \a activity to the top of the activity stack and start
   //! it. Before starting the activity, the topmost activity is paused.
   //!
   void startActivity(std::unique_ptr<Activity> activity);

   //!
   //! Push the provided \a activity to the top of the activity stack and start
   //! it. A flag is set to tell the runtime, that the calling activity expects
   //! a result. Before starting the activity, the topmost activity is paused.
   //!
   void startActivityForResult(std::unique_ptr<Activity> activity, uint8_t key);

   //!
   //!  Stops the topmost Activity and resumes the activity below.
   //!
   void stopActivity();

   //!
   //! Add a \a worker to the workerlist
   //!
   void addWorker(Worker *const worker);

   //!
   //! Get a reference to the Adafruit_SSD1306 display instance
   //!
   Adafruit_SSD1306 &display();

private:
   //!
   //! Helper function which pushes the provided \a activityExecution on top of
   //! the stack
   void pushActivity(std::unique_ptr<ActivityExecution> activityExecution);

private:
   std::unique_ptr<InputModule> inputModule_;
   Adafruit_SSD1306 display_;
   ByteStack resultBytes_;
   std::stack<std::unique_ptr<ActivityExecution>> activityStack;
   std::list<Worker *> workerList;
};
}  // namespace ActivityGUI

#endif