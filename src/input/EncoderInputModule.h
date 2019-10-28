#ifndef _ENCODER_INPUT_MODULE_h
#define _ENCODER_INPUT_MODULE_h

#include <Button.h>
#include <Encoder.h>

#include "InputModule.h"


namespace ActivityGUI
{
class EncoderInputModule : public InputModule
{
public:
   EncoderInputModule(Button button, Encoder encoder);
   void runOnce() override;

private:
   Button button_;
   Encoder encoder_;
};

}  // namespace ActivityGUI

#endif