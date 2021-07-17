#ifndef _BIND_MODE_OPTIONS_HPP
#define _BIND_MODE_OPTIONS_HPP

namespace vind
{
    namespace options {
        void focus_nearest_textarea() {
        }
    }


            // @test
            cache_request_->AddProperty(UIA_IsTextPatternAvailablePropertyId) ;
            cache_request_->AddProperty(UIA_IsValuePatternAvailablePropertyId) ;
            cache_request_->AddProperty(UIA_HasKeyboardFocusPropertyId) ;


    static auto is_text_area(uiauto::SmartElement& elem) {
        BOOL flag ;
        if(SUCCEEDED(elem->get_CachedHasKeyboardFocus(&flag))) {
            if(flag == TRUE) {
                return true ;
            }
        }

        VARIANT val ; 
        if(SUCCEEDED(elem->GetCachedPropertyValue(UIA_IsTextPatternAvailablePropertyId, &val))) {
            if(val.vt != VT_BOOL) {
                std::cout << "NON BOOL\n" ;
                VariantClear(&val) ;
                return false ;
            }

            if(val.boolVal == VARIANT_FALSE) {
                VariantClear(&val) ;
                return false ;
            }
            VariantClear(&val) ;

            if(VARIANT val2 ; SUCCEEDED(elem->GetCachedPropertyValue(UIA_IsValuePatternAvailablePropertyId, &val2))) {
                if(val2.vt != VT_BOOL) {
                    std::cout << "NON BOOL 2\n" ;
                    VariantClear(&val2) ;
                    return false ;
                }

                auto ret = val2.boolVal == VARIANT_TRUE ;
                if(ret) std::cout << "DETECTED\n" ;
                VariantClear(&val2) ;
                return ret ;
            }
            std::cout << "FAILED2\n" ;
        }
        std::cout << "FAILED\n" ;
        return false ;
    }
}

#endif
