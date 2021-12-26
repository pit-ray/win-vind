#ifndef _UIA_HPP
#define _UIA_HPP

#include "uiafwd.hpp"

#include <vector>


namespace vind
{
    namespace util
    {
        SmartElement update_element(
                const SmartElement& elem,
                const SmartCacheReq& request) ;

        void add_property(
                const SmartCacheReq& request,
                PROPERTYID id) ;

        void change_scope(
                const SmartCacheReq& request,
                TreeScope scope) ;

        SmartCacheReq clone(const SmartCacheReq& request) ;

        template <typename T>
        inline auto change_scope(
                const SmartCacheReq& request,
                T scope) {
            return change_scope(
                    request,
                    static_cast<TreeScope>(scope)) ;
        }

        void switch_mode(
                const SmartCacheReq& request,
                bool fullcontrol) ;

        void get_children(
                const SmartElement& elem,
                std::vector<SmartElement>& children) ;

        bool is_enabled(const SmartElement& elem) ;
        bool is_offscreen(const SmartElement& elem) ;
    }
}

#endif
