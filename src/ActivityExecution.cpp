/**
 *  Copyright (c) 2017-2019 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 *  full license information.
 */

#include "ActivityExecution.h"

namespace ActivityGUI
{
ActivityExecution::ActivityExecution(std::unique_ptr<Activity> activity,
                                     bool isResultExpected,
                                     uint8_t key)
    : activity_(std::move(activity))
    , isResultExpected_(isResultExpected)
    , resultKey_(key)
{
}

Activity *ActivityExecution::activity() const
{
   return activity_.get();
}

const bool ActivityExecution::isResultExpected() const
{
   return isResultExpected_;
}

const int8_t ActivityExecution::resultKey() const
{
   return resultKey_;
}
}  // namespace ActivityGUI