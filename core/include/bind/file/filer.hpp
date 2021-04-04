#ifndef _FILER_HPP
#define _FILER_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    struct SaveOpenedFile : public BindedFuncWithCreator<SaveOpenedFile> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;

    struct OpenOtherFile : public BindedFuncWithCreator<OpenOtherFile> {
        static void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) ;
        static const std::string sname() noexcept ;
    } ;
}

#endif
