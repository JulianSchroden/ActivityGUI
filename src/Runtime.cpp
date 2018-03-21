/**  
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.  
 *  Licensed under the MIT License. See LICENSE file in the project root for full license information.  
 */  

#include "Runtime.h"
#include <SPI.h>
#include <Wire.h>

#define LED_BUILTIN 2

Adafruit_SSD1306 Runtime::display(LED_BUILTIN);
std::stack<ActivityExecution*>Runtime::activityStack;
std::list<Worker*> Runtime::workerList;
ByteStack Runtime::resultBytes(16);


volatile int  Runtime::encoderCounter            = 0;
volatile long Runtime::currentButtonPressedTime  = 0;
volatile long Runtime::currentButtonReleasedTime = 0;


void Runtime::buttonCallback() {
	if (!digitalRead(BUTTON)) {	
		if((millis() - currentButtonPressedTime) > 100) // software debounce: only respond to button press when it occurs 100ms after the last press
			currentButtonPressedTime = millis();
	}
	else {
		if (millis() - currentButtonReleasedTime > 100) { // software debounce: only respond to button release when it occurs 100ms after the last release
			currentButtonReleasedTime = millis();
		}
	}
}

void Runtime::encoderCallBack() {
	static int lastA    = 0;
	       int currentA = digitalRead(ENCODER_A);

    if (currentA != lastA) {
        if (currentA != digitalRead(ENCODER_B)) {
            encoderCounter++;
        }
        else {
            encoderCounter--;
        }
        lastA = currentA;
    }
}


Runtime &Runtime::getInstance() {
	static Runtime runtime;
	return runtime;
}


Adafruit_SSD1306 &Runtime::getDisplay() {
	return display;
}


Runtime::Runtime() {
	pinMode(BUTTON, INPUT_PULLUP); // when the Button is pushed, it results in a LOW reading
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


void Runtime::runOnce() {
	static int  lastEncoderCounter    = 0;
	static long lastButtonPressedTime = 0;

	// check encoder counter and trigger onScroll callback, when encoder has been turned
	int scrollDifference = (encoderCounter - lastEncoderCounter);
	if (scrollDifference != 0) {
		if (!activityStack.empty()) {
			activityStack.top()->getActivity()->onScroll(scrollDifference);
		}
		lastEncoderCounter = encoderCounter;
	}

	// check button timestamps
	if (currentButtonPressedTime != lastButtonPressedTime) { //
		if (!digitalRead(BUTTON)) { // if button is still pressed
			if ((millis() - currentButtonPressedTime) > 1000) {
				if (!activityStack.empty()) {
					activityStack.top()->getActivity()->onLongClick(); // trigger onLongClick callback
				}
				lastButtonPressedTime = currentButtonPressedTime;
			}
		}
		else {
			if ((currentButtonReleasedTime - currentButtonPressedTime) >10) {
				if (!activityStack.empty()) {
					activityStack.top()->getActivity()->onClick(); // trigger onClick callback
				}
				lastButtonPressedTime = currentButtonPressedTime;
			}
		}			
	}
	// execute the workers in the workerList
	if(workerList.size() > 0) {
		std::list<Worker*>::iterator workerIt = workerList.begin();
		while(workerIt != workerList.end()) {
			switch((*workerIt)->getState()) { // check the worker's state
				case Worker::STATE_RUNNING:
					(*workerIt)->runOnce(); // call runOnce callback
					workerIt++;
					break;
				case Worker::STATE_TERMINATED:
					workerList.erase(workerIt++); // remove worker once it has been terminated
					break;
				default:
					workerIt++;
					break;
			}
		}

	}
	yield();
	
}

void Runtime::pushActivity(ActivityExecution *execution) {
	if (!activityStack.empty()) {
		activityStack.top()->getActivity()->onPause();
	}
	activityStack.push(execution);
	activityStack.top()->getActivity()->onStart();
}

void Runtime::startActivity(Activity *const activity) {
	ActivityExecution *execution=new ActivityExecution(activity);
	pushActivity(execution);
}

void Runtime::startActivityForResult(Activity *const activity, int8_t key) {
	ActivityExecution *execution=new ActivityExecution(activity, key);
	pushActivity(execution);
}


void Runtime::stopActivity() {
	if (activityStack.size() > 1) {
		ActivityExecution *execution = activityStack.top();
		Activity *activity = execution->getActivity();

		// set a result if the underlying Activity expects it
		if(execution->isResultExpected()){
			resultBytes.reset(); // reset the container to prevent interferences with previous data
			activity->setResult(resultBytes); // pass result container
		}

		// call onDestroy and remove activityExecution from stack
		activity->onDestroy();
		activityStack.pop();
		
		// pass result to Activity if it is expected
		if(execution->isResultExpected()){
			activityStack.top()->getActivity()->onActivityResult(resultBytes, execution->getResultKey()); // pass container and key
		}
		activityStack.top()->getActivity()->onResume();

		// free alloced memory
		delete activity;
		delete execution;
	}
}

void Runtime::addWorker(Worker * const worker) {
	workerList.push_back(worker);
}