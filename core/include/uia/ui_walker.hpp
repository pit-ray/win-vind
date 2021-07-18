#ifndef _UI_WALKER_HPP
#define _UI_WALKER_HPP

#include <initializer_list>
#include <memory>
#include <vector>

#include "uia/uia.hpp"
#include "util/box_2d.hpp"


namespace vind
{
    class UIWalker {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        virtual void setup_cache_request(uiauto::SmartCacheReq req) ;

        virtual bool pinpoint_element(uiauto::SmartElement elem) ;
        virtual bool filter_element(uiauto::SmartElement elem) ;

        bool scan_childrens(
                uiauto::SmartElementArray parents,
                std::vector<uiauto::SmartElement>& elements) ;

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

        virtual void scan(std::vector<uiauto::SmartElement>& elements, HWND hwnd) ;
    } ;
}

#endif
