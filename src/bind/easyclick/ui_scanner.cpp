#include "ui_scanner.hpp"

#include "core/err_logger.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"

#if defined(DEBUG)
#include <iostream>
#endif

namespace vind
{
    UIScanner::UIScanner()
    : UIWalker(UIA_IsKeyboardFocusablePropertyId)
    {}

    bool UIScanner::filter_element(const SmartElement& elem) {
        BOOL flag ;
        if(util::is_failed(elem->get_CachedIsKeyboardFocusable(&flag))) {
            throw RUNTIME_EXCEPT("Could not get the keyboard focusable flag.") ;
        }
        return flag == TRUE ;
    }
}
