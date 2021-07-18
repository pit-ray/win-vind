#include "uia/ui_walker.hpp"

#include "io/screen_metrics.hpp"
#include "util/container.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"

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
          cache_request_(nullptr, uiauto::delete_com)
        {
            IUIAutomationCacheRequest* cr_raw ;
            if(FAILED(cuia_->CreateCacheRequest(&cr_raw))) {
                throw LOGIC_EXCEPT("Could not create IUIAutomationCacheRequest.") ;
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
        pimpl->cache_request_->AddProperty(id) ;
    }
    UIWalker::UIWalker(const std::initializer_list<PROPERTYID>& ids)
    : UIWalker()
    {
        for(const auto& id : ids) {
            pimpl->cache_request_->AddProperty(id) ;
        }
    }
    UIWalker::UIWalker(std::initializer_list<PROPERTYID>&& ids)
    : UIWalker()
    {
        for(auto&& id : ids) {
            pimpl->cache_request_->AddProperty(id) ;
        }
    }

    UIWalker::~UIWalker() noexcept = default ;

    UIWalker::UIWalker(UIWalker&&)            = default ;
    UIWalker& UIWalker::operator=(UIWalker&&) = default ;

    void UIWalker::setup_cache_request(uiauto::SmartCacheReq req) {
        req->AddProperty(UIA_IsEnabledPropertyId) ;
        req->AddProperty(UIA_IsOffscreenPropertyId) ;
        req->AddProperty(UIA_BoundingRectanglePropertyId) ;

        if(FAILED(req->put_AutomationElementMode(
                        AutomationElementMode::AutomationElementMode_None))) {
            throw LOGIC_EXCEPT("Could not initialize UI Automation Element Mode.") ;
        }

        if(FAILED(req->put_TreeScope(TreeScope::TreeScope_Subtree))) {
            throw LOGIC_EXCEPT("Could not initialzie TreeScope.") ;
        }
    }

    bool UIWalker::filter_element(uiauto::SmartElement) {
        return true ;
    }
    bool UIWalker::pinpoint_element(uiauto::SmartElement) {
        return false ;
    }

    bool UIWalker::scan_childrens(
            uiauto::SmartElementArray parents,
            std::vector<uiauto::SmartElement>& elements) {

        int length ;
        parents->get_Length(&length) ;
        if(length == 0) {
            return true ; // continue
        }

        // IUIAutomationElement* elem_raw ;
        // auto elem = uiauto::make_SmartElement(nullptr) ;
        //
        // IUIAutomationElementArray* children_raw ;
        // auto children = uiauto::make_SmartElementArray(nullptr) ;

        BOOL flag ;
        for(int i = 0 ; i < length ; i ++) {
            IUIAutomationElement* elem_raw ;
            if(FAILED(parents->GetElement(i, &elem_raw))) {
                continue ;
            }
            if(!elem_raw) {
                continue ;
            }
            auto elem = uiauto::make_SmartElement(elem_raw) ;
            // elem.reset(elem_raw) ;

            if(FAILED(elem->get_CachedIsEnabled(&flag))) {
                continue ;
            }
            if(!flag) {
                continue ;
            }

            IUIAutomationElementArray* children_raw ;
            if(FAILED(elem->GetCachedChildren(&children_raw))) {
                continue ;
            }
            if(children_raw) {
                auto children = uiauto::make_SmartElementArray(children_raw) ;
                // children.reset(children_raw) ;

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

            if(pinpoint_element(elem)) {
                elements.clear() ;
                elements.push_back(std::move(elem)) ;
                return false ; // break
            }
            if(filter_element(elem)) {
                elements.push_back(std::move(elem)) ;
                continue ;
            }
        }

        return true ; // continue
    }

    void UIWalker::scan(std::vector<uiauto::SmartElement>& elements, HWND hwnd) {
        IUIAutomationElement* elem_raw ;
        if(FAILED(pimpl->cuia_->ElementFromHandle(hwnd, &elem_raw))) {
            throw RUNTIME_EXCEPT("Could not get IUIAutomationElement from HWND by COM method.") ;
        }
        if(!elem_raw) {
            throw RUNTIME_EXCEPT("Could not get UIAutomationElement from HWND.") ;
        }
        auto elem = uiauto::make_SmartElement(elem_raw) ;

        elem_raw = nullptr ;
        if(FAILED(elem->BuildUpdatedCache(pimpl->cache_request_.get(), &elem_raw))) {
            throw RUNTIME_EXCEPT("Could not update caches of UIAutomationElement.") ;
        }
        if(elem_raw != nullptr) {
            std::cout << "Updated\n" ;
            elem.reset(elem_raw) ; //successfully updated
        }

        BOOL flag ;
        if(FAILED(elem->get_CachedIsEnabled(&flag)))  {
            throw RUNTIME_EXCEPT("Could not get a cached IsEnabled flag.") ;
        }
        if(!flag) {
            return ;
        }

        if(FAILED(elem->get_CachedIsOffscreen(&flag))) {
            throw RUNTIME_EXCEPT("Could not get a cached IsOffscreen flag.") ;
        }
        if(flag) {
            return ;
        }

        IUIAutomationElementArray* children_raw ;
        if(SUCCEEDED(elem->GetCachedChildren(&children_raw))) {
            if(children_raw) {
                std::cout << "Start\n" ;
                auto children = uiauto::make_SmartElementArray(children_raw) ;
                scan_childrens(children, elements) ;
                std::cout << "End\n" ;
            }
            else {
                //If the parent element is a leaf in tree.
                if(filter_element(elem) || pinpoint_element(elem)) {
                    elements.push_back(elem) ;
                }
            }
        }
        else {
            throw RUNTIME_EXCEPT("Could not get a cached children as IUIAutomationElementArray.") ;
        }

        elem.reset() ;
        std::cout<< "finished\n" ;
    }
}
