#ifndef _EASY_CLICK_CORE_HPP
#define _EASY_CLICK_CORE_HPP

#include <windows.h>

#include <memory>

#include "core/keycode_def.hpp"

namespace vind
{
    namespace bind
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
            void create_matching_loop(
                    KeyCode sendkey=KEYCODE_UNDEFINED,
                    unsigned int repeat_num=1) const ;

            void reconstruct() ;
        } ;
    }
}

#endif
