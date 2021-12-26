#ifndef _UI_WALKER_HPP
#define _UI_WALKER_HPP

#include <initializer_list>
#include <memory>
#include <unordered_set>
#include <vector>

#include "box_2d.hpp"
#include "uiafwd.hpp"


namespace vind
{
    namespace util
    {
        //
        // IUIAutomationElement interface:
        // https://docs.microsoft.com/en-us/windows/win32/api/uiautomationclient/nn-uiautomationclient-iuiautomationelement
        //
        class UIWalker {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit UIWalker() ;
            explicit UIWalker(PROPERTYID id) ;
            explicit UIWalker(const std::initializer_list<PROPERTYID>& ids) ;
            explicit UIWalker(std::initializer_list<PROPERTYID>&& ids) ;
            virtual ~UIWalker() noexcept ;

            UIWalker(UIWalker&&) ;
            UIWalker& operator=(UIWalker&&) ;

            UIWalker(const UIWalker&)            = delete ;
            UIWalker& operator=(const UIWalker&) = delete ;

            //
            // If you want to use SetFocus or GetCurrentPropertyValue,
            // you must call this function to enable full references.
            // However, there are some overheads.
            //
            void enable_fullcontrol() ;

            // Initialize cache request
            virtual void setup_cache_request(SmartCacheReq request) ;
            const std::unordered_set<PROPERTYID>& get_properties() const noexcept ;

            // Filter functions
            virtual bool pinpoint_element(const SmartElement& elem) ;
            virtual bool filter_element(const SmartElement& elem) ;
            virtual bool filter_root_element(const SmartElement& elem) ;

            // True: otherwise
            // False: Found pinpoint element
            bool append_candidate(
                    SmartElement elem,
                    std::vector<SmartElement>& elements) ;

            // True: otherwise
            // False: Found pinpoint element
            bool scan_childrens(
                    const std::vector<SmartElement>& parents,
                    std::vector<SmartElement>& elements) ;

            // True: otherwise
            // False: Found pinpoint element
            bool scan_element_subtree(
                    const SmartElement& elem,
                    std::vector<SmartElement>& elements) ;

            // with building cache
            void scan(
                    HWND hwnd,
                    std::vector<SmartElement>& elements) ;

            // without building cache
            void scan(
                    const SmartElement& root,
                    std::vector<SmartElement>& elements) ;
        } ;
    }
}

#endif
