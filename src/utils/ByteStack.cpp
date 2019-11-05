/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#include <ActivityGUI/utils/ByteStack.h>

#include <stdint.h>


namespace ActivityGUI
{
ByteStack::ByteStack(uint16_t arraySize)
{
   byteContent = new uint8_t[arraySize];  // allocate memory
   this->arraySize = arraySize;           // store arraySize
}

ByteStack::ByteStack(ByteStack const &other)
{
   // copy content of passed ByteStack
   copy(other);
}

ByteStack::~ByteStack()
{
   // free up allocated memory
   delete[] byteContent;
}

void ByteStack::copy(ByteStack const &other)
{
   // allocate new memory with the size of the passed ByteStack
   byteContent = new uint8_t[other.getSize()];
   // copy content into created array
   uint8_t *otherBytes = other.getBytes();
   for (uint16_t i = 0; i < other.getSize(); i++)
   {
      byteContent[i] = otherBytes[i];
   }
}

ByteStack &ByteStack::operator=(ByteStack const &other)
{
   // prevent self-destruction
   if (this != &other)
   {
      // free up old allocated memory
      delete[] byteContent;
      // copy content of passed ByteStack
      copy(other);
   }
   // return
   return *this;
}

uint8_t *ByteStack::getBytes() const
{
   return byteContent;
}

const uint16_t ByteStack::getSize() const
{
   return arraySize;
}

void ByteStack::reset()
{
   index = 0;
}
}  // namespace ActivityGUI