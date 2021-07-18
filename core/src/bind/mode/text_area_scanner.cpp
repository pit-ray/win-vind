#include "bind/mode/text_area_scanner.hpp"

#include "util/def.hpp"

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
        if(FAILED(elem->get_CachedHasKeyboardFocus(&flag))) {
            return false ;
        }
        return flag == TRUE ;
    }

    bool TextAreaScanner::filter_element(uiauto::SmartElement elem) {
        auto clear = [] (VARIANT* val) {
            VariantClear(val) ;
        } ;
        std::unique_ptr<VARIANT, decltype(clear)> val(new VARIANT(), clear) ;

        if(FAILED(elem->GetCurrentPropertyValue(
                        UIA_IsTextPatternAvailablePropertyId, val.get()))) {
            throw RUNTIME_EXCEPT("Could not get a chached property value: IsTextPatternAvailable.") ;
        }
        if(val->vt != VT_BOOL) {
            return false ;
        }
        if(val->boolVal == VARIANT_FALSE) {
            return false ;
        }

        VariantInit(val.get()) ;
        if(FAILED(elem->GetCurrentPropertyValue(
                        UIA_IsValuePatternAvailablePropertyId, val.get()))) {
            throw RUNTIME_EXCEPT("Could not get a chached property value: IsValuePatternAvailable.") ;
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
