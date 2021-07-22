#ifndef _UIA_HPP
#define _UIA_HPP

#include "uia/cuia.hpp"
#include "uia/uiafwd.hpp"

#include "uia/smartcom.hpp"

#include <vector>


namespace vind
{
    using SmartElement      = SmartCom<IUIAutomationElement> ;
    using SmartElementArray = SmartCom<IUIAutomationElementArray> ;
    using SmartCacheReq     = SmartCom<IUIAutomationCacheRequest> ;

    namespace uiauto {
        const CUIA& get_global_cuia() ;

        void get_children(
                const SmartElement& elem,
                std::vector<SmartElement>& children) ;

        bool is_enabled(const SmartElement& elem) ;
        bool is_offscreen(const SmartElement& elem) ;
    }
}

#endif
