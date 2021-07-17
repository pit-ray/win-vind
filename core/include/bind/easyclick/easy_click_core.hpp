#ifndef _EASY_CLICK_CORE_HPP
#define _EASY_CLICK_CORE_HPP

#include <windows.h>

#include <memory>

#include "key/keycode_def.hpp"

namespace vind
{
    class EasyClickCore {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit EasyClickCore() ;
        virtual ~EasyClickCore() noexcept ;

        EasyClickCore(const EasyClickCore&)            = delete ;
        EasyClickCore& operator=(const EasyClickCore&) = delete ;

        EasyClickCore(EasyClickCore&&) ;
        EasyClickCore& operator=(EasyClickCore&&) ;

        void scan_ui_objects(HWND hwnd) const ;
        void create_matching_loop(KeyCode sendkey=KEYCODE_UNDEFINED) const ;

        void reconstruct() ;
    } ;
}

#endif
