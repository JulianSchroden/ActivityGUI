/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#include "Worker.h"

#include "ActivityRuntime.h"


Worker::Worker()
{
   // add the worker to the ActivityRuntime's worker list
   ActivityRuntime::addWorker(this);
}

Worker::~Worker() {}

void Worker::startWorker()
{
   state = STATE_RUNNING;
}

void Worker::pauseWorker()
{
   state = STATE_PAUSED;
}

void Worker::terminateWorker()
{
   state = STATE_TERMINATED;
}

int8_t Worker::getState() const
{
   return state;
}