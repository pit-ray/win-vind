#include "uia/uia.hpp"

#include "util/def.hpp"
#include "util/winwrap.hpp"

#include <memory>

namespace vind
{
    namespace uiauto {
        const CUIA& get_global_cuia() {
            static CUIA g_cuia{} ;
            return g_cuia ;
        }

        void get_children(
                const SmartElement& elem,
                std::vector<SmartElement>& children) {
            IUIAutomationElementArray* children_raw ;
            if(util::is_failed(elem->GetCachedChildren(&children_raw))) {
                throw RUNTIME_EXCEPT("Could not get a cached children as IUIAutomationElementArray.") ;
            }
            if(!children_raw) {
                return ;
            }

            SmartElementArray elems(children_raw) ;

            int length ;
            elems->get_Length(&length) ;
            if(length == 0) {
                return ;
            }

            for(int i = 0 ; i < length ; i ++) {
                IUIAutomationElement* elem_raw ;

                if(util::is_failed(elems->GetElement(i, &elem_raw))) {
                    throw RUNTIME_EXCEPT("Could not get an element of IUIAutomationElementArray.") ;
                }
                if(!elem_raw) {
                    continue ;
                }
                children.emplace_back(elem_raw) ;
            }
        }

        bool is_enabled(const SmartElement& elem) {
            BOOL flag ;
            if(util::is_failed(elem->get_CachedIsEnabled(&flag))) {
                throw RUNTIME_EXCEPT("Could not get a chached property: IsEnabled.") ;
            }
            return flag == TRUE ;
        }

        bool is_offscreen(const SmartElement& elem) {
            BOOL flag ;
            if(util::is_failed(elem->get_CachedIsOffscreen(&flag))) {
                throw RUNTIME_EXCEPT("Could not get a cached IsOffscreen flag.") ;
            }
            return flag == TRUE ;
        }
    }
}
