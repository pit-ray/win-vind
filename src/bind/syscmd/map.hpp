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
                std::uint16_t count,
                const std::string& args,
                const std::string& prefix="") ;
        } ;

        struct SyscmdNoremap : BindedFuncFlex<SyscmdNoremap> {
            explicit SyscmdNoremap() ;
            static SystemCall sprocess(
                std::uint16_t count,
                const std::string& args,
                const std::string& prefix="") ;
        } ;

        struct SyscmdUnmap : BindedFuncFlex<SyscmdUnmap> {
            explicit SyscmdUnmap() ;
            static SystemCall sprocess(
                std::uint16_t count,
                const std::string& args,
                const std::string& prefix="") ;
        } ;

        struct SyscmdMapclear : BindedFuncFlex<SyscmdMapclear> {
            explicit SyscmdMapclear() ;
            static SystemCall sprocess(
                std::uint16_t count,
                const std::string& args,
                const std::string& prefix="") ;
        } ;
    }
}

#endif
