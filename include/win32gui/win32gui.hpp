#ifndef _WIN32GUI_HPP
#define _WIN32GUI_HPP
#include <windows.h>

namespace Win32GUI
{
    bool is_init(HINSTANCE& hInstance) noexcept ;

    bool is_update() noexcept ;
}

#endif