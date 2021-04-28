#ifndef _UI_SCANNER_HPP
#define _UI_SCANNER_HPP

#include <memory>
#include <vector>

#include "point_2d.hpp"
#include "uia.hpp"

namespace vind
{
    class UIScanner {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit UIScanner() ;
        virtual ~UIScanner() noexcept ;

        UIScanner(const UIScanner&)            = delete ;
        UIScanner& operator=(const UIScanner&) = delete ;

        UIScanner(UIScanner&&) ;
        UIScanner& operator=(UIScanner&&) ;

        const uiauto::SmartCacheReq& get_cache_request() const noexcept ;

        void scan(
                HWND hwnd,
                std::vector<Point2D>& return_positions,
                bool scan_all_thread_window=true) const ;
    } ;
}

#endif
