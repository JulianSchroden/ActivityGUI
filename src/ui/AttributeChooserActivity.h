/**
 *  Copyright (c) 2017-2019 Julian Schroden. All rights reserved.
 *  Licensed under the MIT License. See LICENSE file in the project root for
 *  full license information.
 */

#ifndef _ATTRIBUTE_CHOOSER_ACTIVITY_h
#define _ATTRIBUTE_CHOOSER_ACTIVITY_h

#include <vector>

#include "../utils/AttributeChoice.h"
#include "Activity.h"


namespace ActivityGUI
{
class AttributeChooserActivity : public Activity
{
public:
   //!
   //! Create AttributeChooserActivity instance.
   //! The AttributeChooserActivity creates a list layout using the provided \a
   //! items. The \a selected parameter can be used to select the default value.
   //!
   AttributeChooserActivity(std::vector<AttributeChoice> items,
                            std::string title,
                            uint8_t selected = 0,
                            bool showTitleBar = false,
                            int titleFontScale = 2,
                            bool showArrowHome = false);

   void onScroll(int distance) override;

   void setResult(ByteStack &bytes) override;

protected:
   void drawLayout(DrawMode drawMode = DrawMode::TransferBuffer) override;

private:
   std::vector<AttributeChoice> items_;
   int8_t selectedItem_ = 0;      // index of the selected item
   int8_t lastSelectedItem_ = 0;  // index of the last selected item
};
}  // namespace ActivityGUI

#endif