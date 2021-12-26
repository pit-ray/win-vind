#include "ui_walker.hpp"

#include "core/err_logger.hpp"

#include "container.hpp"
#include "def.hpp"
#include "rect.hpp"
#include "screen_metrics.hpp"
#include "util/cuia.hpp"
#include "util/uia.hpp"
#include "winwrap.hpp"

#include <initializer_list>
#include <stdexcept>
#include <unordered_set>
#include <vector>


namespace vind
{
    namespace util
    {
        struct UIWalker::Impl {
            SmartCacheReq cache_request_ = \
                CUIA::get_instance().create_cache_request() ;
            std::unordered_set<PROPERTYID> properties_{} ;
        } ;

        UIWalker::UIWalker()
        : pimpl(std::make_unique<Impl>())
        {
            setup_cache_request(pimpl->cache_request_) ;
        }
        UIWalker::UIWalker(PROPERTYID id)
        : UIWalker()
        {
            add_property(pimpl->cache_request_, id) ;
            pimpl->properties_.insert(id) ;
        }
        UIWalker::UIWalker(const std::initializer_list<PROPERTYID>& ids)
        : UIWalker()
        {
            for(const auto& id : ids) {
                add_property(pimpl->cache_request_, id) ;
                pimpl->properties_.insert(id) ;
            }
        }
        UIWalker::UIWalker(std::initializer_list<PROPERTYID>&& ids)
        : UIWalker()
        {
            for(auto&& id : ids) {
                add_property(pimpl->cache_request_, id) ;
                pimpl->properties_.insert(id) ;
            }
        }

        UIWalker::~UIWalker() noexcept = default ;

        UIWalker::UIWalker(UIWalker&&)            = default ;
        UIWalker& UIWalker::operator=(UIWalker&&) = default ;


        void UIWalker::enable_fullcontrol() {
            switch_mode(pimpl->cache_request_, true) ;
        }

        void UIWalker::setup_cache_request(SmartCacheReq req) {
            add_property(req, UIA_IsEnabledPropertyId) ;
            add_property(req, UIA_IsOffscreenPropertyId) ;
            add_property(req, UIA_BoundingRectanglePropertyId) ;

            // All property getters is only available for cache.
            switch_mode(req, false) ;

            change_scope(req, TreeScope::TreeScope_Subtree) ;
        }

        const std::unordered_set<PROPERTYID>& UIWalker::get_properties() const noexcept {
            return pimpl->properties_ ;
        }

        bool UIWalker::filter_element(const SmartElement&) {
            return true ; // Pass all elements
        }
        bool UIWalker::pinpoint_element(const SmartElement&) {
            return false ; // Block all elements
        }

        bool UIWalker::filter_root_element(const SmartElement& elem) {
            if(!is_enabled(elem)) {
                return false ;
            }

            if(is_offscreen(elem)) {
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
                if(!is_enabled(elem)) {
                    continue ;
                }

                std::vector<SmartElement> children ;
                get_children(elem, children) ;
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
            get_children(elem, children) ;

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

        void UIWalker::scan(
                HWND hwnd,
                std::vector<SmartElement>& elements) {

            auto root = CUIA::get_instance().get_root_element(hwnd) ;
            root = update_element(root, pimpl->cache_request_) ;
            scan_element_subtree(root, elements) ;
        }

        void UIWalker::scan(
                const SmartElement& root,
                std::vector<SmartElement>& elements) {

            scan_element_subtree(root, elements) ;
        }
    }
}
