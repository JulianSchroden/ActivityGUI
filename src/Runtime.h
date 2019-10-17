/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#ifndef _RUNTIME_h
#define _RUNTIME_h

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include <list>
#include <stack>

#include "ActivityExecution.h"
#include "Worker.h"
#include "ui/Activity.h"
#include "utils/ByteStack.h"

#define BUTTON 26
#define ENCODER_A 25
#define ENCODER_B 33

class Runtime
{
private:
   static Adafruit_SSD1306 display;
   static std::stack<ActivityExecution *> activityStack;
   static std::list<Worker *> workerList;
   static ByteStack resultBytes;

   static volatile int encoderCounter;
   static volatile long currentButtonPressedTime;
   static volatile long currentButtonReleasedTime;

   // interrupt callbacks
   static void encoderCallBack();
   static void buttonCallback();

   /**
    * Pushes an ActivityExecution on top of the stack
    * @param execution   a pointer to an ActivityExecution instance
    */
   static void pushActivity(ActivityExecution *execution);

   // private constructor to prevent calls from outside
   Runtime();

public:
   /**
    *  perform the Runtime's work
    */
   static void runOnce();

   /**
    *  @return a reference to the Runtime instance
    */
   static Runtime &getInstance();
   /**
    *  @return a reference to the Adafruit_SSD1306 instance
    */
   static Adafruit_SSD1306 &getDisplay();

   /**
    * Pauses the current topmost activity, pushes the provided activity to the
    * stack and starts it
    * @param activity   a const Pointer to an Activity instance
    */
   static void startActivity(Activity *const activity);

   /**
    * Pauses the current topmost activity, pushes the provided activity to the
    * stack and starts it. A flag is set, that the calling activity expects a
    * result from the started activity.
    * @param activity   a const pointer to an Activity instance
    * @param key        a int8_t value >= 0 to identify the activity
    */
   static void startActivityForResult(Activity *const activity, int8_t key);

   /**
    *  Stops the topmost Activity, removes it from the stack and resumes
    * activity below
    */
   static void stopActivity();

   /**
    * Add a worker to the workerList
    * @param worker   a const pointer to a Worker instance
    */
   static void addWorker(Worker *const worker);
};
#endif