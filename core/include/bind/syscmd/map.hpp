#ifndef _MAP_HPP
#define _MAP_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct SyscmdMap : BindedFuncCreator<SyscmdMap> {
        explicit SyscmdMap() ;
        static void sprocess(
                const mode::Mode mode = mode::Mode::None,
                const std::string& args="",
                bool reload_config=false) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct SyscmdNoremap : BindedFuncCreator<SyscmdNoremap> {
        explicit SyscmdNoremap() ;
        static void sprocess(
                const mode::Mode mode = mode::Mode::None,
                const std::string& args="",
                bool reload_config=false) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct SyscmdUnmap: BindedFuncCreator<SyscmdUnmap> {
        explicit SyscmdUnmap() ;
        static void sprocess(
                const mode::Mode mode = mode::Mode::None,
                const std::string& args="",
                bool reload_config=false) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct SyscmdMapclear : BindedFuncCreator<SyscmdMapclear> {
        explicit SyscmdMapclear() ;
        static void sprocess(
                const mode::Mode mode = mode::Mode::None,
                bool reload_config=false) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
