#ifndef _WIN32GUI_HPP
#define _WIN32GUI_HPP
#include <windows.h>

namespace Win32GUI
{
    bool init(HINSTANCE& hInstance) noexcept ;

    bool update() noexcept ;
}

#endif