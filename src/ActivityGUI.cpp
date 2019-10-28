/**
 *  Copyright (c) 2017-2019 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 *  full license information.
 */

#include "ActivityGUI.h"

namespace ActivityGUI
{
Runtime::Runtime(InputModule inputModule, Adafruit_SSD1306 display)
    : inputModule_(std::move(inputModule))
    , display_(std::move(display))
    , resultBytes_(16)
{
   inputModule_.onClick(
       []() { activityStack.top()->getActivity()->onClick(); });

   inputModule_.onLongClick(
       []() { activityStack.top()->getActivity()->onLongClick(); });
   
   inputModule_.onScroll([this](int distance) {
      activityStack.top()->getActivity()->onScroll(distance);
   });
}

void Runtime::runOnce()
{
   inputModule_.runOnce();

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
   pushActivity(new ActivityExecution(std::move(activity));
}

void Runtime::startActivityForResult(Activity *const activity, int8_t key)
{
   pushActivity(new ActivityExecution(std::move(activity), key));
}

void Runtime::stopActivity()
{
   if (activityStack.size() > 1)
   {
      ActivityExecution *execution = activityStack.top();
      Activity *activity = execution->getActivity();

      // set a result if the underlying Activity expects it
      if (execution->isResultExpected())
      {
         resultBytes.reset();  // reset the container to prevent interferences
                               // with previous data
         activity->setResult(resultBytes);  // pass result container
      }

      // call onDestroy and remove activityExecution from stack
      activity->onDestroy();
      activityStack.pop();

      // pass result to Activity if it is expected
      if (execution->isResultExpected())
      {
         activityStack.top()->getActivity()->onActivityResult(
             resultBytes, execution->getResultKey());  // pass container and key
      }
      activityStack.top()->getActivity()->onResume();

      // free alloced memory
      delete activity;
      delete execution;
   }
}

void Runtime::addWorker(Worker *const worker)
{
   workerList.push_back(worker);
}

Adafruit_SSD1306 &Runtime::display()
{
   return display_;
}

void Runtime::pushActivity(std::unique_ptr<ActivityExecution> activityExecution)
{
   if (!activityStack.empty())
   {
      activityStack.top()->getActivity()->onPause();
   }
   activityStack.push(std::move(activityExecution));
   activityStack.top()->getActivity()->setRuntime(this);
   activityStack.top()->getActivity()->onStart();
}

}  // namespace ActivityGUI