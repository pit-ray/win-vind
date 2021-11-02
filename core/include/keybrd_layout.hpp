#ifndef _KEYBRD_LAYOUT_HPP
#define _KEYBRD_LAYOUT_HPP

#include <filesystem>
#include <string>

#include <windows.h>

namespace vind
{
    namespace keybrd_layout {
        std::filesystem::path get_layout_filepath(LANGID id) ;
    }
}


#endif
