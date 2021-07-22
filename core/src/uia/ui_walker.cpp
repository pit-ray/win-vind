#include "uia/ui_walker.hpp"

#include "err_logger.hpp"
#include "io/screen_metrics.hpp"
#include "util/container.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"
#include "util/winwrap.hpp"

#include <future>
#include <initializer_list>
#include <stdexcept>
#include <vector>

#if defined(DEBUG)
#include <iostream>
#include "util/debug.hpp"
#endif


namespace vind
{
    struct UIWalker::Impl {
        const CUIA& cuia_ ;
        SmartCacheReq cache_request_ ;

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


    void UIWalker::enable_fullcontrol() {
        if(util::is_failed(pimpl->cache_request_->put_AutomationElementMode(
                        AutomationElementMode::AutomationElementMode_Full))) {
            throw LOGIC_EXCEPT("Could not set UI Automation Element Mode to full-reference mode.") ;
        }
    }

    void UIWalker::setup_cache_request(SmartCacheReq req) {
        if(util::is_failed(req->AddProperty(UIA_IsEnabledPropertyId))) {
            throw RUNTIME_EXCEPT("Could not add property: IsEnabled") ;
        }
        if(util::is_failed(req->AddProperty(UIA_IsOffscreenPropertyId))) {
            throw RUNTIME_EXCEPT("Could not add property: IsOffscreen") ;
        }
        if(util::is_failed(req->AddProperty(UIA_BoundingRectanglePropertyId))) {
            throw RUNTIME_EXCEPT("Could not add property: BoundingRectangle") ;
        }

        // All property getters is only available for cache.
        if(util::is_failed(req->put_AutomationElementMode(
                        AutomationElementMode::AutomationElementMode_None))) {
            throw LOGIC_EXCEPT("Could not initialize UI Automation Element Mode to read-only mode.") ;
        }

        if(util::is_failed(pimpl->cache_request_->put_TreeScope(
                        TreeScope::TreeScope_Subtree))) {
            throw LOGIC_EXCEPT("Could not initialzie TreeScope.") ;
        }
    }

    SmartCacheReq UIWalker::clone_cache_request() const {
        IUIAutomationCacheRequest* req ;
        if(util::is_failed(pimpl->cache_request_->Clone(&req))) {
            throw RUNTIME_EXCEPT("IUIAutomationCacheRequest::Clone failed.") ;
        }
        if(!req) {
            throw RUNTIME_EXCEPT("Invalid CacheRequest clone") ;
        }
        return SmartCacheReq(req) ;
    }

    bool UIWalker::filter_element(const SmartElement&) {
        return true ; // Pass all elements
    }
    bool UIWalker::pinpoint_element(const SmartElement&) {
        return false ; // Block all elements
    }

    bool UIWalker::filter_root_element(const SmartElement& elem) {
        if(!uiauto::is_enabled(elem)) {
            return false ;
        }

        if(uiauto::is_offscreen(elem)) {
            return false ;
        }

        return true ;
    }

    bool UIWalker::append_candidate(
            SmartElement elem,
            std::vector<SmartElement>& elements) {

        if(filter_element(elem)) {
            if(pinpoint_element(elem)) {
                elements.clear() ;
                elements.push_back(std::move(elem)) ;
                return false ; // Found the pinpoint element
            }
            elements.push_back(std::move(elem)) ;
        }
        return true ;
    }


    bool UIWalker::scan_childrens(
            const std::vector<SmartElement>& parents,
            std::vector<SmartElement>& elements) {

        for(const auto& elem : parents) {
            if(!uiauto::is_enabled(elem)) {
                continue ;
            }

            std::vector<SmartElement> children ;
            uiauto::get_children(elem, children) ;
            if(!children.empty()) {
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

            if(!append_candidate(elem, elements)) {
                return false ;
            }
        }

        return true ; // continue
    }

    bool UIWalker::scan_element_subtree(
            const SmartElement& elem,
            std::vector<SmartElement>& elements) {

        std::vector<SmartElement> children ;
        uiauto::get_children(elem, children) ;

        if(children.empty()) {
            return append_candidate(elem, elements) ;
        }

        auto before_size = elements.size() ;
        if(!scan_childrens(children, elements)) {
            return false ;
        }
        if(elements.size() == before_size) {
            return append_candidate(elem, elements) ;
        }
        return true ;
    }

    SmartElement UIWalker::update_element(const SmartElement& elem) {
        IUIAutomationElement* elem_raw ;
        if(util::is_failed(elem->BuildUpdatedCache(pimpl->cache_request_.get(), &elem_raw))) {
            throw RUNTIME_EXCEPT("Could not update caches of UIAutomationElement.") ;
        }

        if(!elem_raw) {
            throw RUNTIME_EXCEPT("Could not build the caches of all elements.") ;
        }

        return SmartElement(elem_raw) ;
    }


    void UIWalker::scan(
            HWND hwnd,
            std::vector<SmartElement>& elements) {

        auto root = get_root_element(hwnd) ;
        root = update_element(root) ;
        scan_element_subtree(root, elements) ;
    }

    void UIWalker::scan(
            const SmartElement& root,
            std::vector<SmartElement>& elements) {

        scan_element_subtree(root, elements) ;
    }

    SmartElement UIWalker::get_root_element(HWND hwnd) {
        IUIAutomationElement* elem_raw ;
        if(util::is_failed(pimpl->cuia_->ElementFromHandle(hwnd, &elem_raw))) {
            throw RUNTIME_EXCEPT("Could not get IUIAutomationElement from HWND by COM method.") ;
        }
        if(!elem_raw) {
            throw RUNTIME_EXCEPT("Could not get UIAutomationElement from HWND.") ;
        }
        return SmartElement(elem_raw) ;
    }
}
