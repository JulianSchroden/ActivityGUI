#include "InputModule.h"

namespace ActivityGUI
{
void InputModule::onClick(std::function<void()> clickCallback)
{
   this->clickCallback_ = clickCallback;
}

void InputModule::onLongClick(std::function<void()> longClickCallback)
{
   this->longClickCallback_ = longClickCallback;
}

void InputModule::onScroll(std::function<void(int)> scrollCallback)
{
   this->scrollCallback_ = scrollCallback;
}

void InputModule::onClick()
{
   if (clickCallback_) clickCallback_();
}

void InputModule::onLongClick()
{
   if (longClickCallback_) longClickCallback_();
}

void InputModule::onScroll(int scrollDistance)
{
   if (scrollCallback_) scrollCallback_(scrollDistance);
}

}  // namespace ActivityGUI