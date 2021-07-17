#include "uia/ui_walker.hpp"

#include "io/screen_metrics.hpp"
#include "util/container.hpp"
#include "util/def.hpp"
#include "util/rect.hpp"

#include <initializer_list>
#include <vector>


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

    void UIWalker::setup_cache_request(uiauto::SmartCacheReq& req) {
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

    bool UIWalker::is_target(uiauto::SmartElement&) {
        return true ;
    }

    void UIWalker::scan_childrens(
            uiauto::SmartElementArray& parents,
            std::vector<Box2D>& obj_list,
            const RECT& root_rect) {

        int length ;
        parents->get_Length(&length) ;
        if(length == 0) {
            return ;
        }

        IUIAutomationElement* elem_raw ;
        auto elem = uiauto::make_SmartElement(nullptr) ;

        IUIAutomationElementArray* children_raw ;
        auto children = uiauto::make_SmartElementArray(nullptr) ;

        BOOL flag ;
        for(int i = 0 ; i < length ; i ++) {
            if(FAILED(parents->GetElement(i, &elem_raw))) {
                continue ;
            }
            if(!elem_raw) {
                continue ;
            }
            elem.reset(elem_raw) ;

            if(FAILED(elem->get_CachedIsEnabled(&flag))) {
                continue ;
            }
            if(!flag) {
                continue ;
            }

            if(FAILED(elem->GetCachedChildren(&children_raw))) {
                continue ;
            }
            if(children_raw != nullptr) {
                children.reset(children_raw) ;

                //recursive calling
                scan_childrens(children, obj_list, root_rect) ;

                continue ;
            }

            //scan GUI objects only at leaves in tree.
            try {
                RECT rect ;
                if(FAILED(elem->get_CachedBoundingRectangle(&rect))) {
                    throw std::runtime_error("Could not get the a rectangle of element.") ;
                }

                // ignore windows that are larger than the root rectangle.
                if(!util::is_fully_in_range(rect, root_rect)) {
                    throw std::runtime_error("The rectangle of element exists in a invalid range.") ;
                }

                obj_list.emplace_back(rect) ;
            }
            catch(const std::runtime_error&) {
                continue ;
            }
        }
    }

    void UIWalker::scan(std::vector<Box2D>& obj_list, HWND hwnd) {
        IUIAutomationElement* elem_raw ;
        if(FAILED(pimpl->cuia_->ElementFromHandle(hwnd, &elem_raw))) {
            throw RUNTIME_EXCEPT("Could not get IUIAutomationElement from HWND by COM method.") ;
        }
        if(!elem_raw) {
            throw RUNTIME_EXCEPT("Could not get UIAutomationElement from HWND.") ;
        }
        auto elem = uiauto::make_SmartElement(elem_raw) ;

        if(FAILED(elem->BuildUpdatedCache(pimpl->cache_request_.get(), &elem_raw))) {
            throw RUNTIME_EXCEPT("Could not update caches of UIAutomationElement.") ;
        }
        if(elem_raw != nullptr) {
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

        RECT rect ;
        if(!GetWindowRect(hwnd, &rect)) {
            throw RUNTIME_EXCEPT("Could not get the rectangle of window.") ;
        }

        IUIAutomationElementArray* children_raw ;
        if(SUCCEEDED(elem->GetCachedChildren(&children_raw))) {
            if(children_raw) {
                auto children = uiauto::make_SmartElementArray(children_raw) ;
                scan_childrens(children, obj_list, rect) ;
            }
            else {
                //If the parent element is a leaf in tree.
                if(is_target(elem)) {
                    obj_list.emplace_back(std::move(rect)) ;
                }
            }
        }
        else {
            throw RUNTIME_EXCEPT("Could not get a cached children as IUIAutomationElementArray.") ;
        }

        util::remove_deplication(obj_list) ;
    }
}
