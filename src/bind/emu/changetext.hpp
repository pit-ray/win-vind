#ifndef _EDI_CHANGE_HPP
#define _EDI_CHANGE_HPP

#include "bind/bindedfunc.hpp"
#include "bind/emu/deltext.hpp"
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

        struct ChangeLine : public DeleteLineUntilEOLBase<ChangeLine> {
            explicit ChangeLine() ;
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ChangeChar : public ChangeBaseCreator<ChangeChar> {
            explicit ChangeChar() ;
            static void sprocess(unsigned int count=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ChangeUntilEOL : public DeleteLineUntilEOLBase<ChangeUntilEOL> {
            explicit ChangeUntilEOL() ;
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
