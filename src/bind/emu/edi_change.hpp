#ifndef _EDI_CHANGE_HPP
#define _EDI_CHANGE_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace textanalyze {
        struct SelRes ;
    }

    struct ChangeHighlightText : public BindedFuncCreator<ChangeHighlightText> {
        explicit ChangeHighlightText() ;
        static void sprocess() ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;
    } ;

    struct ChangeLine : public BindedFuncCreator<ChangeLine> {
        explicit ChangeLine() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;

        bool is_for_changing_text() const noexcept override {
            return true ;
        }
    } ;

    struct ChangeChar : public BindedFuncCreator<ChangeChar> {
        explicit ChangeChar() ;
        static void sprocess(unsigned int repeat_num=1) ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;

        bool is_for_changing_text() const noexcept override {
            return true ;
        }
    } ;

    struct ChangeUntilEOL : public BindedFuncCreator<ChangeUntilEOL> {
        explicit ChangeUntilEOL() ;
        static void sprocess(
                unsigned int repeat_num=1,
                const textanalyze::SelRes* const exres=nullptr) ;
        static void sprocess(core::NTypeLogger& parent_lgr) ;
        static void sprocess(const core::CharLogger& parent_lgr) ;

        bool is_for_changing_text() const noexcept override {
            return true ;
        }
    } ;
}

#endif
