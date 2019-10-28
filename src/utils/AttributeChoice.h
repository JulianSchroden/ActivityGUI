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
       : title_(std::move(title)), value_(value)
   {
   }

   inline std::string title() const
   {
      return title_;
   }

   inline int16_t value() const
   {
      return value_;
   }

private:
   std::string title_;
   int16_t value_;
};
}  // namespace ActivityGUI

#endif