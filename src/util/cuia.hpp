#ifndef _CUIA_HPP
#define _CUIA_HPP

#include "uiafwd.hpp"

namespace vind
{
    namespace util
    {
        class CUIA {
        private:
            IUIAutomation* cuia_ ;

            explicit CUIA() ;
            virtual ~CUIA() noexcept ;

        public:

            static CUIA& get_instance() ;

            IUIAutomation* get() const noexcept ;
            operator IUIAutomation*() const noexcept ;
            operator bool() const noexcept ;
            IUIAutomation* operator->() const noexcept ;

            SmartCacheReq create_cache_request() ;

            SmartElement get_root_element(HWND hwnd) ;

            CUIA(const CUIA&) = delete ;
            CUIA(CUIA&&)      = delete ;
            CUIA& operator=(const CUIA&) = delete ;
            CUIA& operator=(CUIA&&)      = delete ;
        } ;
    }
}

#endif
