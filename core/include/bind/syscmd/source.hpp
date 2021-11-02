#ifndef _SOURCE_HPP
#define _SOURCE_HPP

#include "bind/binded_func_creator.hpp"
#include "path.hpp"

namespace vind
{
    struct SyscmdSource : BindedFuncCreator<SyscmdSource> {
        explicit SyscmdSource() ;
        static void sprocess(
                const std::filesystem::path& path=path::RC(),
                bool reload_config=false,
                bool start_from_default=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
