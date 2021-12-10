#ifndef _EDI_CHANGE_HPP
#define _EDI_CHANGE_HPP

#include "bind/binded_func.hpp"
#include "changebase.hpp"

namespace vind
{
    namespace bind
    {
        struct SelectedTextResult ;

        struct ChangeHighlightText : public BindedFuncVoid<ChangeHighlightText> {
            explicit ChangeHighlightText() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ChangeLine : public ChangeBaseCreator<ChangeLine> {
            explicit ChangeLine() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ChangeChar : public ChangeBaseCreator<ChangeChar> {
            explicit ChangeChar() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ChangeUntilEOL : public ChangeBaseCreator<ChangeUntilEOL> {
            explicit ChangeUntilEOL() ;
            static void sprocess(
                    unsigned int repeat_num=1,
                    const SelectedTextResult* const exres=nullptr) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
