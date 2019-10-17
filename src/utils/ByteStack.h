/**
 *  Copyright (c) 2017-2018 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 * full license information.
 */

#ifndef _BYTE_STACK_h
#define _BYTE_STACK_h

#include <stdint.h>

namespace ActivityGUI
{
class ByteStack
{
private:
   uint8_t *byteContent;
   uint16_t index = 0;
   uint16_t arraySize;

   /**
    *  Helper method to copy content from other ByteStack
    *  @param other   a reference to an other ByteStack
    */
   void copy(ByteStack const &other);

public:
   /**
    *  ByteStack constructor
    *  @param arraySize   the amount of bytes which should be allocated
    */
   ByteStack(uint16_t arraySize);

   /**
    * ByteStack copy constructor
    * @param other   a reference to an other ByteStack instace which should be
    * copied
    */
   ByteStack(ByteStack const &other);

   /**
    *   ByteStack destrucor
    */
   ~ByteStack();

   /**
    * overloading assignment operator
    * @param other   a reference to an other ByteStack instace which should be
    * copied
    * @return this object instance to be able to chain assignments
    */
   ByteStack &operator=(ByteStack const &other);

   /**
    *  Pushs an attribute to the top of the stack, if there is space left
    *  @return boolean value, if the attribute has been added successfully
    */
   template <class T>
   bool push(T data)
   {
      uint16_t size = sizeof(T);
      // check if there is still space left for the passed attribute
      if (arraySize - index >= size)
      {
         // point with an uint8_t pointer at the address of the attribute
         uint8_t *bytes = (uint8_t *)&data;
         // loop over the attribute bytes and store them in byteContent
         for (uint16_t i = 0; i < size; i++)
         {
            byteContent[index + i] = bytes[i];
         }
         // increment index by attribute size
         index += size;
         // return true after attribute has been added successfully
         return true;
      }
      // no space left
      return false;
   }

   /**
    *  Fetches the topmost attribute from the stack and removes it afterwards
    *  @return the fetched attribute
    */
   template <class T>
   T pop()
   {
      uint16_t size = sizeof(T);
      if (index - size >= 0)
      {
         /* fetch result from array by:
          *   1. moving the byteContent pointer to right postion,
          *   2. casting the pointer to Pointer to T
          *   3. dereferencing the Pointer
          */
         T result = *(T *)(byteContent + index - size);
         // decrement index by size of T
         index -= size;
         // returning the result
         return result;
      }
   }

   /** get the content of the byteStack
    *  @return a pointer to the bytes. No const pointer is used because some
    * libraries don't support them
    */
   uint8_t *getBytes() const;

   /**
    *  get the size of the byteStack
    *  @return the size of the byteStack
    */
   const uint16_t getSize() const;

   /**
    *  resets the byteStack by moving the index to 0
    */
   void reset();
};
}  // namespace ActivityGUI

#endif