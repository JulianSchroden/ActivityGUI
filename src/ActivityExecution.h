/**
 *  Copyright (c) 2017-2019 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 *  full license information.
 */

#ifndef _ACTIVITY_EXECUTION_h
#define _ACTIVITY_EXECUTION_h

#include <memory>

#include "ui/Activity.h"


namespace ActivityGUI
{
class ActivityExecution
{
public:
   //!
   //! Create an ActivityExecution instance of the provided \a acitivity.
   //! When the calling Activity expects a result, \a isResultExpected needs to
   //! be set to true. The \a key is passed to the onActivityResult function of
   //! the calling Activity, so it is easy to identify the result.
   //!
   ActivityExecution(std::unique_ptr<Activity> activity,
                     bool isResultExpected = false,
                     uint8_t key = 0);

   //!
   //! Get a pointer to the activity
   //!
   Activity *activity() const;

   //!
   //! Check if the activity should return a result.
   //!
   const bool isResultExpected() const;

   //!
   //! Get the result key
   //!
   const int8_t resultKey() const;

private:
   std::unique_ptr<Activity> activity_;
   bool isResultExpected_;
   uint8_t resultKey_;
};
}  // namespace ActivityGUI

#endif