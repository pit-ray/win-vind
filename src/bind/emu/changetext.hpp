#ifndef _EDI_CHANGE_HPP
#define _EDI_CHANGE_HPP

#include "bind/bindedfunc.hpp"
#include "changebase.hpp"

namespace vind
{
    namespace bind
    {
        struct SelectedTextResult ;

        struct ChangeHighlightText : public BindedFuncVoid<ChangeHighlightText> {
            explicit ChangeHighlightText() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;

        struct ChangeLine : public ChangeBaseCreator<ChangeLine> {
            explicit ChangeLine() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;

        struct ChangeChar : public ChangeBaseCreator<ChangeChar> {
            explicit ChangeChar() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;

        struct ChangeUntilEOL : public ChangeBaseCreator<ChangeUntilEOL> {
            explicit ChangeUntilEOL() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;
    }
}

#endif
