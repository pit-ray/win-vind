#ifndef _CHANGE_MODE_HPP
#define _CHANGE_MODE_HPP

#include "bind/binded_func.hpp"
#include "text_area_scanner.hpp"

namespace vind
{
    namespace bind
    {
        struct ToGUINormal : public BindedFuncVoid<ToGUINormal> {
            explicit ToGUINormal() ;
            static void sprocess(bool vclmodeout=true) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ToResident : public BindedFuncVoid<ToResident> {
            explicit ToResident() ;
            static void sprocess(bool vclmodeout=true) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ToGUIVisual : public BindedFuncVoid<ToGUIVisual> {
            explicit ToGUIVisual() ;
            static void sprocess(bool vclmodeout=true) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        class ToEdiNormal : public BindedFuncVoid<ToEdiNormal> {
        private:
            static TextAreaScanner scanner_ ;

        public:
            explicit ToEdiNormal() ;
            static void sprocess(bool vclmodeout=true) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ToInsert : public BindedFuncVoid<ToInsert> {
            explicit ToInsert() ;
            static void sprocess(bool vclmodeout=true) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ToEdiVisual : public BindedFuncVoid<ToEdiVisual> {
            explicit ToEdiVisual() ;
            static void sprocess(bool vclmodeout=true) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ToEdiVisualLine : public BindedFuncVoid<ToEdiVisualLine> {
            explicit ToEdiVisualLine() ;
            static void sprocess(bool vclmodeout=true) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
