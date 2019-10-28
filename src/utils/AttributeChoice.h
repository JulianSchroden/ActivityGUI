/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#ifndef _ATTRIBUTE_CHOICE_h
#define _ATTRIBUTE_CHOICE_h

#include <stdint.h>

#include <string>

namespace ActivityGUI
{
class AttributeChoice
{
public:
   AttributeChoice(std::string title, int16_t value)
       : title(std::move(title)), value(value)
   {
   }

   inline std::string title() const
   {
      return title;
   }

   inline int16_t value() const
   {
      return value;
   }

private:
   std::string title;
   int16_t value;
};
}  // namespace ActivityGUI

#endif