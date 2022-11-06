#ifndef _MKDIR_HPP
#define _MKDIR_HPP

#include "bind/bindedfunc.hpp"

#include <filesystem>

namespace vind
{
    namespace bind
    {
        struct MakeDir : public BindedFuncVoid<MakeDir> {
            explicit MakeDir() ;
            static void sprocess(
                std::uint16_t count, const std::string& args="New Folder") ;
        } ;
    }
}

#endif
