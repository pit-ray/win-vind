#ifndef _EDI_REPLACE_HPP
#define _EDI_REPLACE_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    struct EdiNReplaceChar : public BindedFuncCreator<EdiNReplaceChar> {
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EdiNReplaceSequence : public BindedFuncCreator<EdiNReplaceSequence> {
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EdiSwitchCharCase : public BindedFuncCreator<EdiSwitchCharCase> {
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
