#ifndef _KEYBRD_LAYOUT_HPP
#define _KEYBRD_LAYOUT_HPP

#include <filesystem>
#include <string>

#include <windows.h>

namespace vind
{
    namespace core {
        std::filesystem::path get_keylayout_filepath(LANGID id) ;
    }
}


#endif
