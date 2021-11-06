#include "uia.hpp"

#include "def.hpp"
#include "winwrap.hpp"

#include <memory>

namespace vind
{
    namespace uiauto {
        const CUIA& get_global_cuia() {
            static CUIA g_cuia{} ;
            return g_cuia ;
        }

        SmartCacheReq create_cache_request() {
            IUIAutomationCacheRequest* cr_raw ;
            if(util::is_failed(get_global_cuia()->CreateCacheRequest(&cr_raw))) {
                throw RUNTIME_EXCEPT("Could not create IUIAutomationCacheRequest.") ;
            }
            if(!cr_raw) {
                throw RUNTIME_EXCEPT("Could not get IUIAutomationCacheRequest properly.") ;
            }
            return SmartCacheReq(cr_raw) ;
        }

        SmartElement get_root_element(HWND hwnd) {
            IUIAutomationElement* elem_raw ;
            if(util::is_failed(get_global_cuia()->ElementFromHandle(hwnd, &elem_raw))) {
                throw RUNTIME_EXCEPT("Could not get IUIAutomationElement from HWND by COM method.") ;
            }
            if(!elem_raw) {
                throw RUNTIME_EXCEPT("Could not get UIAutomationElement from HWND.") ;
            }
            return SmartElement(elem_raw) ;
        }

        void add_property(
                const SmartCacheReq& request,
                PROPERTYID id) {

            if(util::is_failed(request->AddProperty(id))) {
                throw RUNTIME_EXCEPT("Could not add an uia property: " + std::to_string(id)) ;
            }
        }

        void change_scope(
                const SmartCacheReq& request,
                TreeScope scope) {

            if(util::is_failed(request->put_TreeScope(scope))) {
                throw LOGIC_EXCEPT("Could not initialzie TreeScope.") ;
            }
        }

        void switch_mode(
                const SmartCacheReq& request,
                bool fullcontrol) {

            auto mode = fullcontrol ?
                            AutomationElementMode::AutomationElementMode_Full :
                            AutomationElementMode::AutomationElementMode_None ;

            if(util::is_failed(request->put_AutomationElementMode(mode))) {
                throw LOGIC_EXCEPT("Could not set UI Automation Element mode to " + std::to_string(mode)) ;
            }
        }

        SmartCacheReq clone(const SmartCacheReq& request) {
            IUIAutomationCacheRequest* req ;
            if(util::is_failed(request->Clone(&req))) {
                throw RUNTIME_EXCEPT("IUIAutomationCacheRequest::Clone failed.") ;
            }
            if(!req) {
                throw RUNTIME_EXCEPT("Invalid CacheRequest clone") ;
            }
            return SmartCacheReq(req) ;
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

        SmartElement update_element(const SmartElement& elem, const SmartCacheReq& request) {
            IUIAutomationElement* elem_raw ;
            if(util::is_failed(elem->BuildUpdatedCache(request.get(), &elem_raw))) {
                throw RUNTIME_EXCEPT("Could not update caches of UIAutomationElement.") ;
            }

            if(!elem_raw) {
                throw RUNTIME_EXCEPT("Could not build the caches of all elements.") ;
            }

            return SmartElement(elem_raw) ;
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
