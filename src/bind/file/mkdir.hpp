#ifndef _MKDIR_HPP
#define _MKDIR_HPP

#include "bind/binded_func_creator.hpp"

#include <filesystem>

namespace vind
{
    struct MakeDir : public BindedFuncCreator<MakeDir> {
        explicit MakeDir() ;
        static void sprocess(const std::filesystem::path& path="New Folder") ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
