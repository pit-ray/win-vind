#ifndef _SOURCE_HPP
#define _SOURCE_HPP

#include "bind/binded_func_creator.hpp"
#include "core/path.hpp"

namespace vind
{
    namespace bind
    {
        struct SyscmdSource : BindedFuncCreator<SyscmdSource> {
            explicit SyscmdSource() ;
            static void sprocess(
                    const std::filesystem::path& path=core::RC(),
                    bool reload_config=false,
                    bool start_from_default=true) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
