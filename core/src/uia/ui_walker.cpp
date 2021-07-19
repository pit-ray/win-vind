#include "uia/ui_walker.hpp"

#include "io/screen_metrics.hpp"
#include "util/container.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"
#include "util/winwrap.hpp"

#include <initializer_list>
#include <vector>

#if defined(DEBUG)
#include <iostream>
#endif


namespace vind
{
    struct UIWalker::Impl {
        const uiauto::CUIA& cuia_ ;
        uiauto::SmartCacheReq cache_request_ ;

        explicit Impl()
        : cuia_(uiauto::get_global_cuia()),
          cache_request_()
        {
            IUIAutomationCacheRequest* cr_raw ;
            if(util::is_failed(cuia_->CreateCacheRequest(&cr_raw))) {
                throw RUNTIME_EXCEPT("Could not create IUIAutomationCacheRequest.") ;
            }
            if(!cr_raw) {
                throw RUNTIME_EXCEPT("Could not get IUIAutomationCacheRequest properly.") ;
            }
            cache_request_.reset(cr_raw) ;
        }
    } ;

    UIWalker::UIWalker()
    : pimpl(std::make_unique<Impl>())
    {
        setup_cache_request(pimpl->cache_request_) ;
    }
    UIWalker::UIWalker(PROPERTYID id)
    : UIWalker()
    {
        if(util::is_failed(pimpl->cache_request_->AddProperty(id))) {
            throw RUNTIME_EXCEPT("Could not add property " + std::to_string(id)) ;
        }
    }
    UIWalker::UIWalker(const std::initializer_list<PROPERTYID>& ids)
    : UIWalker()
    {
        for(const auto& id : ids) {
            if(util::is_failed(pimpl->cache_request_->AddProperty(id))) {
                throw RUNTIME_EXCEPT("Could not add property " + std::to_string(id)) ;
            }
        }
    }
    UIWalker::UIWalker(std::initializer_list<PROPERTYID>&& ids)
    : UIWalker()
    {
        for(auto&& id : ids) {
            if(util::is_failed(pimpl->cache_request_->AddProperty(id))) {
                throw RUNTIME_EXCEPT("Could not add property " + std::to_string(id)) ;
            }
        }
    }

    UIWalker::~UIWalker() noexcept = default ;

    UIWalker::UIWalker(UIWalker&&)            = default ;
    UIWalker& UIWalker::operator=(UIWalker&&) = default ;

    void UIWalker::setup_cache_request(uiauto::SmartCacheReq req) {
        if(util::is_failed(req->AddProperty(UIA_IsEnabledPropertyId))) {
            throw RUNTIME_EXCEPT("Could not add property: IsEnabled") ;
        }
        if(util::is_failed(req->AddProperty(UIA_IsOffscreenPropertyId))) {
            throw RUNTIME_EXCEPT("Could not add property: IsOffscreen") ;
        }
        if(util::is_failed(req->AddProperty(UIA_BoundingRectanglePropertyId))) {
            throw RUNTIME_EXCEPT("Could not add property: BoundingRectangle") ;
        }

        if(util::is_failed(req->put_AutomationElementMode(
                        AutomationElementMode::AutomationElementMode_None))) {
            throw LOGIC_EXCEPT("Could not initialize UI Automation Element Mode to read-only mode.") ;
        }

        if(util::is_failed(req->put_TreeScope(TreeScope::TreeScope_Subtree))) {
            throw LOGIC_EXCEPT("Could not initialzie TreeScope.") ;
        }
    }

    bool UIWalker::filter_element(uiauto::SmartElement) {
        return true ;
    }
    bool UIWalker::pinpoint_element(uiauto::SmartElement) {
        return false ;
    }

    void UIWalker::enable_fullcontrol() {
        if(util::is_failed(pimpl->cache_request_->put_AutomationElementMode(
                        AutomationElementMode::AutomationElementMode_Full))) {
            throw LOGIC_EXCEPT("Could not set UI Automation Element Mode to full-reference mode.") ;
        }
    }

    bool UIWalker::scan_childrens(
            uiauto::SmartElementArray parents,
            std::vector<uiauto::SmartElement>& elements) {

        int length ;
        parents->get_Length(&length) ;
        if(length == 0) {
            return true ; // continue
        }

        BOOL flag ;
        for(int i = 0 ; i < length ; i ++) {
            IUIAutomationElement* elem_raw ;
            if(util::is_failed(parents->GetElement(i, &elem_raw))) {
                continue ;
            }
            if(!elem_raw) {
                continue ;
            }
            uiauto::SmartElement elem(elem_raw) ;

            if(util::is_failed(elem->get_CachedIsEnabled(&flag))) {
                continue ;
            }
            if(!flag) {
                continue ;
            }

            IUIAutomationElementArray* children_raw ;
            if(util::is_failed(elem->GetCachedChildren(&children_raw))) {
                continue ;
            }
            if(children_raw) {
                uiauto::SmartElementArray children(children_raw) ;

                auto before_size = elements.size() ;

                //recursive calling
                if(!scan_childrens(children, elements)) {
                    return false ;
                }

                // Detected children as target
                if(elements.size() > before_size) {
                    continue ;
                }
            }

            if(filter_element(elem)) {
                if(pinpoint_element(elem)) {
                    elements.clear() ;
                    elements.push_back(std::move(elem)) ;
                    return false ; // break
                }

                elements.push_back(std::move(elem)) ;
                continue ;
            }
        }

        return true ; // continue
    }

    void UIWalker::scan(HWND hwnd, std::vector<uiauto::SmartElement>& elements) {
        IUIAutomationElement* elem_raw ;
        if(util::is_failed(pimpl->cuia_->ElementFromHandle(hwnd, &elem_raw))) {
            throw RUNTIME_EXCEPT("Could not get IUIAutomationElement from HWND by COM method.") ;
        }
        if(!elem_raw) {
            throw RUNTIME_EXCEPT("Could not get UIAutomationElement from HWND.") ;
        }
        uiauto::SmartElement elem(elem_raw) ;

        elem_raw = nullptr ;
        if(util::is_failed(elem->BuildUpdatedCache(pimpl->cache_request_.get(), &elem_raw))) {
            throw RUNTIME_EXCEPT("Could not update caches of UIAutomationElement.") ;
        }
        if(elem_raw != nullptr) {
            elem.reset(elem_raw) ; //successfully updated
        }

        BOOL flag ;
        if(util::is_failed(elem->get_CachedIsEnabled(&flag)))  {
            throw RUNTIME_EXCEPT("Could not get a cached IsEnabled flag.") ;
        }
        if(!flag) {
            return ;
        }

        if(util::is_failed(elem->get_CachedIsOffscreen(&flag))) {
            throw RUNTIME_EXCEPT("Could not get a cached IsOffscreen flag.") ;
        }
        if(flag) {
            return ;
        }

        IUIAutomationElementArray* children_raw ;
        if(util::is_failed(elem->GetCachedChildren(&children_raw))) {
            throw RUNTIME_EXCEPT("Could not get a cached children as IUIAutomationElementArray.") ;
        }

        if(children_raw) {
            uiauto::SmartElementArray children(children_raw) ;
            scan_childrens(children, elements) ;
        }
        else {
            //If the parent element is a leaf in tree.
            if(filter_element(elem) || pinpoint_element(elem)) {
                elements.push_back(elem) ;
            }
        }
    }
}
