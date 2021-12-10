#ifndef _MAP_HPP
#define _MAP_HPP

#include "bind/binded_func.hpp"


namespace vind
{
    namespace bind
    {
        struct SyscmdMap : BindedFuncFlex<SyscmdMap> {
            explicit SyscmdMap() ;
            static core::SystemCall sprocess(
                    const core::Mode mode = core::Mode::UNDEFINED,
                    const std::string& args="") ;
            static core::SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static core::SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SyscmdNoremap : BindedFuncFlex<SyscmdNoremap> {
            explicit SyscmdNoremap() ;
            static core::SystemCall sprocess(
                    const core::Mode mode = core::Mode::UNDEFINED,
                    const std::string& args="") ;
            static core::SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static core::SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SyscmdUnmap : BindedFuncFlex<SyscmdUnmap> {
            explicit SyscmdUnmap() ;
            static core::SystemCall sprocess(
                    const core::Mode mode = core::Mode::UNDEFINED,
                    const std::string& args="") ;
            static core::SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static core::SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SyscmdMapclear : BindedFuncFlex<SyscmdMapclear> {
            explicit SyscmdMapclear() ;
            static core::SystemCall sprocess(
                    const core::Mode mode = core::Mode::UNDEFINED) ;
            static core::SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static core::SystemCall sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
