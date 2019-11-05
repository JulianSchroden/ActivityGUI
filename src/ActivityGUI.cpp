/**
 *  Copyright (c) 2017-2019 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 *  full license information.
 */

#include <ActivityGUI/ActivityGUI.h>

#include "shim/make_unique.h"


namespace ActivityGUI
{
Runtime::Runtime(std::unique_ptr<InputModule> inputModule,
                 std::unique_ptr<Adafruit_SSD1306> display)
    : inputModule_(std::move(inputModule))
    , display_(std::move(display))
    , resultBytes_(16)
{
   inputModule_->onClick([this]() {
      if (!activityStack.empty())
      {
         activityStack.top()->activity()->onClick();
      }
   });

   inputModule_->onLongClick([this]() {
      if (!activityStack.empty())
      {
         activityStack.top()->activity()->onLongClick();
      }
   });

   inputModule_->onScroll([this](int distance) {
      if (!activityStack.empty())
      {
         activityStack.top()->activity()->onScroll(distance);
      }
   });
}

void Runtime::runOnce()
{
   inputModule_->runOnce();

#if 0
   // execute the workers in the workerList
   if (workerList.size() > 0)
   {
      std::list<Worker *>::iterator workerIt = workerList.begin();
      while (workerIt != workerList.end())
      {
         switch ((*workerIt)->getState())
         {  // check the worker's state
            case Worker::STATE_RUNNING:
               (*workerIt)->runOnce();  // call runOnce callback
               workerIt++;
               break;
            case Worker::STATE_TERMINATED:
               workerList.erase(
                   workerIt++);  // remove worker once it has been terminated
               break;
            default:
               workerIt++;
               break;
         }
      }
   }
#endif
   yield();
}

void Runtime::startActivity(std::unique_ptr<Activity> activity)
{
   pushActivity(std::make_unique<ActivityExecution>(std::move(activity)));
}

void Runtime::startActivityForResult(std::unique_ptr<Activity> activity,
                                     uint8_t key)
{
   pushActivity(
       std::make_unique<ActivityExecution>(std::move(activity), true, key));
}

void Runtime::stopActivity()
{
   if (activityStack.size() > 1)
   {
      auto activityExecution = std::move(activityStack.top());
      activityStack.pop();

      Activity *activity = activityExecution->activity();

      // set a result if the underlying Activity expects it
      if (activityExecution->isResultExpected())
      {
         resultBytes_.reset();  // reset the container to prevent interferences
                                // with previous data
         activity->setResult(resultBytes_);
      }

      activity->onDestroy();

      // pass result to Activity if it is expected
      if (activityExecution->isResultExpected())
      {
         activityStack.top()->activity()->onActivityResult(
             resultBytes_, activityExecution->resultKey());
      }
      activityStack.top()->activity()->onResume();
   }
}

void Runtime::addWorker(Worker *const worker)
{
   workerList.push_back(worker);
}

Adafruit_SSD1306 &Runtime::display()
{
   return *display_.get();
}

void Runtime::pushActivity(std::unique_ptr<ActivityExecution> activityExecution)
{
   if (!activityStack.empty())
   {
      activityStack.top()->activity()->onPause();
   }
   activityStack.push(std::move(activityExecution));
   activityStack.top()->activity()->setRuntime(this);
   activityStack.top()->activity()->onStart();
}

}  // namespace ActivityGUI