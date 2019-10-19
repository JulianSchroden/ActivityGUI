/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#include "ActivityRuntime.h"

#include <SPI.h>
#include <Wire.h>


#define LED_BUILTIN 2

namespace ActivityGUI
{
Adafruit_SSD1306 ActivityRuntime::display(LED_BUILTIN);
std::stack<ActivityExecution *> ActivityRuntime::activityStack;
std::list<Worker *> ActivityRuntime::workerList;
ByteStack ActivityRuntime::resultBytes(16);

volatile int ActivityRuntime::encoderCounter = 0;
volatile long ActivityRuntime::currentButtonPressedTime = 0;
volatile long ActivityRuntime::currentButtonReleasedTime = 0;

ActivityRuntime &ActivityRuntime::getInstance()
{
   static ActivityRuntime ActivityRuntime;
   return ActivityRuntime;
}

Adafruit_SSD1306 &ActivityRuntime::getDisplay()
{
   return display;
}

void ActivityRuntime::runOnce()
{
   static int lastEncoderCounter = 0;
   static long lastButtonPressedTime = 0;

   // check encoder counter and trigger onScroll callback, when encoder has been
   // turned
   int scrollDifference = (encoderCounter - lastEncoderCounter);
   if (scrollDifference != 0)
   {
      if (!activityStack.empty())
      {
         activityStack.top()->getActivity()->onScroll(scrollDifference);
      }
      lastEncoderCounter = encoderCounter;
   }

   // check button timestamps
   if (currentButtonPressedTime != lastButtonPressedTime)
   {  //
      if (!digitalRead(BUTTON))
      {  // if button is still pressed
         if ((millis() - currentButtonPressedTime) > 1000)
         {
            if (!activityStack.empty())
            {
               activityStack.top()
                   ->getActivity()
                   ->onLongClick();  // trigger onLongClick callback
            }
            lastButtonPressedTime = currentButtonPressedTime;
         }
      }
      else
      {
         if ((currentButtonReleasedTime - currentButtonPressedTime) > 10)
         {
            if (!activityStack.empty())
            {
               activityStack.top()
                   ->getActivity()
                   ->onClick();  // trigger onClick callback
            }
            lastButtonPressedTime = currentButtonPressedTime;
         }
      }
   }
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
   yield();
}

void ActivityRuntime::startActivity(Activity *const activity)
{
   ActivityExecution *execution = new ActivityExecution(activity);
   pushActivity(execution);
}

void ActivityRuntime::startActivityForResult(Activity *const activity,
                                             int8_t key)
{
   ActivityExecution *execution = new ActivityExecution(activity, key);
   pushActivity(execution);
}

void ActivityRuntime::stopActivity()
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

void ActivityRuntime::addWorker(Worker *const worker)
{
   workerList.push_back(worker);
}

ActivityRuntime::ActivityRuntime()
{
   pinMode(
       BUTTON,
       INPUT_PULLUP);  // when the Button is pushed, it results in a LOW reading
   pinMode(ENCODER_A, INPUT);
   pinMode(ENCODER_B, INPUT);
   attachInterrupt(digitalPinToInterrupt(BUTTON), buttonCallback, CHANGE);
   attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoderCallBack, CHANGE);

   // display init
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
   display.display();
   delay(1000);
   display.clearDisplay();
}

void ActivityRuntime::buttonCallback()
{
   if (!digitalRead(BUTTON))
   {
      if ((millis() - currentButtonPressedTime) >
          100)  // software debounce: only respond to button press when it
                // occurs 100ms after the last press
         currentButtonPressedTime = millis();
   }
   else
   {
      if (millis() - currentButtonReleasedTime > 100)
      {  // software debounce: only respond to button release when it occurs
         // 100ms after the last release
         currentButtonReleasedTime = millis();
      }
   }
}

void ActivityRuntime::encoderCallBack()
{
   static int lastA = 0;
   int currentA = digitalRead(ENCODER_A);

   if (currentA != lastA)
   {
      if (currentA != digitalRead(ENCODER_B))
      {
         encoderCounter++;
      }
      else
      {
         encoderCounter--;
      }
      lastA = currentA;
   }
}

void ActivityRuntime::pushActivity(ActivityExecution *execution)
{
   if (!activityStack.empty())
   {
      activityStack.top()->getActivity()->onPause();
   }
   activityStack.push(execution);
   activityStack.top()->getActivity()->onStart();
}

}  // namespace ActivityGUI