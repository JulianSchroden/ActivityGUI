#include "EncoderInputModule.h"

#include <Arduino.h>

namespace ActivityGUI
{
EncoderInputModule::EncoderInputModule(Button button, Encoder encoder)
    : button_(std::move(button)), encoder_(std::move(encoder))
{
   button_.onClick([this]() { onClick(); });
   button_.onLongClick([this]() { onLongClick(); });
   encoder_.onScroll([this](int distance) { onScroll(distance); });
}

void EncoderInputModule::runOnce()
{
   button_.update();
   encoder_.update();
}

}  // namespace ActivityGUI