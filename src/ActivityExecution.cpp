/**  
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.  
 *  Licensed under the MIT License. See LICENSE file in the project root for full license information.  
 */  

#include "ActivityExecution.h"

ActivityExecution::ActivityExecution(Activity *const activity, int8_t key)
:activity(activity) {
    if(key != -1){
        resultKey      = key;
        resultExpected = true;
    }
}

Activity *ActivityExecution::getActivity() const {
    return activity;
}

const bool ActivityExecution::isResultExpected() const {
    return resultExpected;
}

const int8_t ActivityExecution::getResultKey() const {
    return resultKey;
}