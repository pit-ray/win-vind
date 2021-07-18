#include "bind/mode/text_area_scanner.hpp"

#include "err_logger.hpp"
#include "util/def.hpp"
#include "util/winwrap.hpp"

#if defined(DEBUG)
#include <iostream>
#include "util/string.hpp"
#endif


namespace vind
{
    TextAreaScanner::TextAreaScanner()
    : UIWalker{
        UIA_IsTextPatternAvailablePropertyId,
        UIA_IsValuePatternAvailablePropertyId,
        UIA_HasKeyboardFocusPropertyId
    }
    {}

    bool TextAreaScanner::pinpoint_element(uiauto::SmartElement elem) {
        BOOL flag ;
        if(util::is_failed(elem->get_CachedHasKeyboardFocus(&flag))) {
            // throw RUNTIME_EXCEPT("Could not get a cached property value: HasKeyboardFocus.") ;
            return false ;
        }
        return flag == TRUE ;
    }

    bool TextAreaScanner::filter_element(uiauto::SmartElement elem) {
        VARIANT val_instance ;

        auto clear = [] (VARIANT* val) {
            VariantClear(val) ;
        } ;
        std::unique_ptr<VARIANT, decltype(clear)> val(&val_instance, clear) ;

        if(util::is_failed(elem->GetCachedPropertyValue(
                        UIA_IsTextPatternAvailablePropertyId, val.get()))) {
            // throw RUNTIME_EXCEPT("Could not get a chached property value: IsTextPatternAvailable.") ;
            return false ;
        }
        if(val->vt != VT_BOOL) {
            return false ;
        }
        if(val->boolVal == VARIANT_FALSE) {
            return false ;
        }
        val.reset() ; // release VARIANT

        val.reset(&val_instance) ; // re-set local VARIANT
        if(util::is_failed(elem->GetCachedPropertyValue(
                        UIA_IsValuePatternAvailablePropertyId, val.get()))) {
            // throw RUNTIME_EXCEPT("Could not get a chached property value: IsValuePatternAvailable.") ;
            return false ;
        }
        if(val->vt != VT_BOOL) {
            return false ;
        }
        if(val->boolVal == VARIANT_FALSE) {
            return false ;
        }

        return true ;
    }
}
