#ifndef _EDI_REPLACE_HPP
#define _EDI_REPLACE_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    struct ReplaceChar : public BindedFuncCreator<ReplaceChar> {
        explicit ReplaceChar() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;

        bool is_for_changing_text() const noexcept override {
            return true ;
        }
    } ;

    struct ReplaceSequence : public BindedFuncCreator<ReplaceSequence> {
        explicit ReplaceSequence() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;

        bool is_for_changing_text() const noexcept override {
            return true ;
        }
    } ;

    struct SwitchCharCase : public BindedFuncCreator<SwitchCharCase> {
        explicit SwitchCharCase() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;

        bool is_for_changing_text() const noexcept override {
            return true ;
        }
    } ;
}

#endif
