#ifndef _SOURCE_HPP
#define _SOURCE_HPP

#include "bind/bindedfunc.hpp"
#include "core/path.hpp"

namespace vind
{
    namespace bind
    {
        struct SyscmdSource : BindedFuncFlex<SyscmdSource> {
            explicit SyscmdSource() ;
            static SystemCall sprocess(
                    const std::filesystem::path& path=core::RC(),
                    bool start_from_default=true) ;
            static SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
