#include "text_area_scanner.hpp"

#include "core/err_logger.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"

#if defined(DEBUG)
#include <iostream>
#include "util/string.hpp"
#endif


namespace vind
{
    namespace bind
    {
        TextAreaScanner::TextAreaScanner()
        : UIWalker{
            UIA_IsTextPatternAvailablePropertyId,
            UIA_IsValuePatternAvailablePropertyId,
            UIA_HasKeyboardFocusPropertyId,
            UIA_ValueIsReadOnlyPropertyId
        }
        {
            UIWalker::enable_fullcontrol() ;
        }

        bool TextAreaScanner::pinpoint_element(const util::SmartElement& elem) {
            BOOL flag ;
            if(util::is_failed(elem->get_CachedHasKeyboardFocus(&flag))) {
                throw RUNTIME_EXCEPT("Could not get a cached property value: HasKeyboardFocus.") ;
            }
            return flag == TRUE ;
        }

        bool TextAreaScanner::filter_element(const util::SmartElement& elem) {
            VARIANT val ;

            if(util::is_failed(elem->GetCachedPropertyValue(
                            UIA_ValueIsReadOnlyPropertyId, &val))) {
                throw RUNTIME_EXCEPT("Could not get a chached property value: IsReadOnly.") ;
            }
            if(val.vt != VT_BOOL) {
                return false ;
            }
            if(val.boolVal == VARIANT_TRUE) {
                return false ;
            }

            if(util::is_failed(elem->GetCachedPropertyValue(
                            UIA_IsTextPatternAvailablePropertyId, &val))) {
                throw RUNTIME_EXCEPT("Could not get a chached property value: IsTextPatternAvailable.") ;
            }
            if(val.vt != VT_BOOL) {
                return false ;
            }
            if(val.boolVal == VARIANT_FALSE) {
                return false ;
            }

            if(util::is_failed(elem->GetCachedPropertyValue(
                            UIA_IsValuePatternAvailablePropertyId, &val))) {
                throw RUNTIME_EXCEPT("Could not get a chached property value: IsValuePatternAvailable.") ;
            }
            if(val.vt != VT_BOOL) {
                return false ;
            }
            if(val.boolVal == VARIANT_FALSE) {
                return false ;
            }

            return true ;
        }
    }
}
