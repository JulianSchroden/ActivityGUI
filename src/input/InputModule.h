/**
 *  Copyright (c) 2017-2019 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 *  full license information.
 */

#ifndef _INPUT_MODULE_h
#define _INPUT_MODULE_h

#include <functional>

namespace ActivityGUI
{
class InputModule
{
public:
   virtual ~InputModule() = default;

   void onClick(std::function<void()> clickCallback);
   void onLongClick(std::function<void()> longClickCallback);
   void onScroll(std::function<void(int)> scrollCallback);

   virtual void runOnce() = 0;  // ToDo: derrive from worker class

protected:
   void onClick();
   void onLongClick();
   void onScroll(int scrollDistance);

private:
   std::function<void()> clickCallback_;
   std::function<void()> longClickCallback_;
   std::function<void(int)> scrollCallback_;
};
}  // namespace ActivityGUI

#endif