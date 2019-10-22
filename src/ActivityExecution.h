/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#ifndef _ACTIVITY_EXECUTION_h
#define _ACTIVITY_EXECUTION_h

#include "ui/Activity.h"

namespace ActivityGUI
{
class ActivityExecution
{
private:
   Activity *activity;
   bool resultExpected = false;
   int8_t resultKey = -1;  // -1 -> no result is expected
public:
   /**
    *  ActivityExecution constructor
    *  @param activity   a const pointer to an activity instance
    *  @param key        optional parameter to identify activity instance
    */
   ActivityExecution(Activity *const activity, int8_t key = -1);

   /**
    * @return a pointer to the activity instance
    */
   Activity *getActivity() const;

   /**
    *  @return true if a result is expected, otherwise false
    */
   const bool isResultExpected() const;

   /**
    *  @return the result key
    */
   const int8_t getResultKey() const;
};
}  // namespace ActivityGUI

#endif