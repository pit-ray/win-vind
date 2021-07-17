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

        virtual void setup_cache_request(uiauto::SmartCacheReq& req) ;

        virtual bool is_target(uiauto::SmartElement& elem) ;

        void scan_childrens(
                uiauto::SmartElementArray& parents,
                std::vector<Box2D>& obj_list,
                const RECT& root_rect) ;


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

        virtual void scan(std::vector<Box2D>& obj_list, HWND hwnd) ;
    } ;
}

#endif
