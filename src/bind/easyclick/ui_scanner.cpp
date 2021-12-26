#include "ui_scanner.hpp"

#include "core/errlogger.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace bind
    {
        UIScanner::UIScanner()
        : UIWalker(UIA_IsKeyboardFocusablePropertyId)
        {}

        bool UIScanner::filter_element(const util::SmartElement& elem) {
            BOOL flag ;
            if(util::is_failed(elem->get_CachedIsKeyboardFocusable(&flag))) {
                throw RUNTIME_EXCEPT("Could not get the keyboard focusable flag.") ;
            }
            return flag == TRUE ;
        }
    }
}
