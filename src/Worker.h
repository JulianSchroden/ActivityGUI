/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#ifndef _WORKER_H
#define _WORKER_H

#include <stdint.h>
namespace ActivityGUI
{
class Worker
{
private:
   int8_t state = STATE_INITIAL;

public:
   Worker();
   virtual ~Worker();
   static const int8_t STATE_INITIAL = 1;
   static const int8_t STATE_RUNNING = 2;
   static const int8_t STATE_PAUSED = 3;
   static const int8_t STATE_TERMINATED = 4;

   /**
    *  changes state to STATE_RUNNING
    */
   void startWorker();

   /**
    *  changes state to STATE_PAUSED
    */
   void pauseWorker();

   /**
    *  changes state to STATE_TERMINATED
    */
   void terminateWorker();

   /**
    *  @return the worker's state
    */
   int8_t getState() const;

   /**
    *  abstract callback which needs to be implemented by the derived classes
    */
   virtual void runOnce() = 0;
};
}  // namespace ActivityGUI

#endif