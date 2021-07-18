#include "bind/easyclick/ui_scanner.hpp"

#include "util/def.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace vind
{
    UIScanner::UIScanner()
    : UIWalker(UIA_IsKeyboardFocusablePropertyId)
    {}

    bool UIScanner::filter_element(uiauto::SmartElement elem) {
        BOOL flag ;
        if(FAILED(elem->get_CachedIsKeyboardFocusable(&flag))) {
            throw RUNTIME_EXCEPT("Could not get the keyboard focusable flag.") ;
        }
        return flag == TRUE ;
    }
}
