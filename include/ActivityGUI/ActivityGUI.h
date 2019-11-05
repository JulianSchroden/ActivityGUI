/**
 *  Copyright (c) 2017-2019 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 *  full license information.
 */

#ifndef _ACTIVITY_GUI_h
#define _ACTIVITY_GUI_h

#include <ActivityGUI/input/InputModule.h>
#include <ActivityGUI/ui/Activity.h>
#include <ActivityGUI/utils/ByteStack.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SimpleWorker/WorkerPool.h>

#include <list>
#include <memory>
#include <stack>


namespace ActivityGUI
{
class Runtime
{
public:
   //!
   //! Create a Runtime instance.
   //!
   Runtime(std::unique_ptr<InputModule> inputModule,
           std::unique_ptr<Adafruit_SSD1306> display);

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
   //! Get a reference to the Adafruit_SSD1306 display instance.
   //!
   Adafruit_SSD1306 &display();

   //!
   //! Get a reference to the WorkerPool.
   //!
   SimpleWorker::WorkerPool &workerPool();

private:
   //!
   //! Helper class to manage the activity executions.
   //!
   class ActivityExecution
   {
   public:
      //!
      //! Create an ActivityExecution instance of the provided \a acitivity.
      //! When the calling Activity expects a result, \a isResultExpected needs
      //! to be set to true. The \a key is passed to the onActivityResult
      //! function of the calling Activity, so it is easy to identify the
      //! result.
      //!
      ActivityExecution(std::unique_ptr<Activity> activity,
                        bool isResultExpected = false,
                        uint8_t key = 0);

      //!
      //! Get a pointer to the activity.
      //!
      Activity *activity() const;

      //!
      //! Check if the activity should return a result.
      //!
      bool isResultExpected() const;

      //!
      //! Get the result key.
      //!
      int8_t resultKey() const;

   private:
      std::unique_ptr<Activity> activity_;
      bool isResultExpected_;
      uint8_t resultKey_;
   };

   //!
   //! Helper function which pushes the provided \a activityExecution on top of
   //! the stack
   void pushActivity(
       std::unique_ptr<Runtime::ActivityExecution> activityExecution);

private:
   std::unique_ptr<InputModule> inputModule_;
   std::unique_ptr<Adafruit_SSD1306> display_;
   ByteStack resultBytes_;
   std::stack<std::unique_ptr<Runtime::ActivityExecution>> activityStack;
   SimpleWorker::WorkerPool workerPool_;
};
}  // namespace ActivityGUI

#endif