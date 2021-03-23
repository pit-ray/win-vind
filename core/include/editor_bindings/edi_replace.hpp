#ifndef _EDI_REPLACE_HPP
#define _EDI_REPLACE_HPP

#include "binded_func_with_creator.hpp"

namespace vind
{
    struct EdiNReplaceChar : public BindedFuncWithCreator<EdiNReplaceChar> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EdiNReplaceSequence : public BindedFuncWithCreator<EdiNReplaceSequence> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct EdiSwitchCharCase : public BindedFuncWithCreator<EdiSwitchCharCase> {
        static void sprocess(
                const bool first_call,
                const unsigned int repeat_num,
                VKCLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
