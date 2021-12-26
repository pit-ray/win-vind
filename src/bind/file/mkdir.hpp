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
            static void sprocess(const std::filesystem::path& path="New Folder") ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
