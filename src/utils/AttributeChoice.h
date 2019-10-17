/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#ifndef _ATTRIBUTE_CHOICE_h
#define _ATTRIBUTE_CHOICE_h

#include <stdint.h>

class AttributeChoice
{
   String title;
   int16_t value;

public:
   AttributeChoice(String title, int16_t value)
   {
      this->title = title;
      this->value = value;
   }
   inline String getTitle() const
   {
      return title;
   }
   inline int16_t getValue() const
   {
      return value;
   }
};

#endif