#ifndef _MAP_HPP
#define _MAP_HPP

#include "bind/bindedfunc.hpp"


namespace vind
{
    namespace bind
    {
        struct SyscmdMap : BindedFuncFlex<SyscmdMap> {
            explicit SyscmdMap() ;
            static SystemCall sprocess(
                    const core::Mode mode = core::Mode::UNDEFINED,
                    const std::string& args="") ;
            static SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SyscmdNoremap : BindedFuncFlex<SyscmdNoremap> {
            explicit SyscmdNoremap() ;
            static SystemCall sprocess(
                    const core::Mode mode = core::Mode::UNDEFINED,
                    const std::string& args="") ;
            static SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SyscmdUnmap : BindedFuncFlex<SyscmdUnmap> {
            explicit SyscmdUnmap() ;
            static SystemCall sprocess(
                    const core::Mode mode = core::Mode::UNDEFINED,
                    const std::string& args="") ;
            static SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SyscmdMapclear : BindedFuncFlex<SyscmdMapclear> {
            explicit SyscmdMapclear() ;
            static SystemCall sprocess(
                    const core::Mode mode = core::Mode::UNDEFINED) ;
            static SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
