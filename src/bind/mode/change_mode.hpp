#ifndef _CHANGE_MODE_HPP
#define _CHANGE_MODE_HPP

#include "bind/binded_func_creator.hpp"
#include "text_area_scanner.hpp"

namespace vind
{
    struct ToGUINormal : public BindedFuncCreator<ToGUINormal> {
        explicit ToGUINormal() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct ToResident : public BindedFuncCreator<ToResident> {
        explicit ToResident() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct ToGUIVisual : public BindedFuncCreator<ToGUIVisual> {
        explicit ToGUIVisual() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    class ToEdiNormal : public BindedFuncCreator<ToEdiNormal> {
    private:
        static TextAreaScanner scanner_ ;

    public:
        explicit ToEdiNormal() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct ToInsert : public BindedFuncCreator<ToInsert> {
        explicit ToInsert() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct ToEdiVisual : public BindedFuncCreator<ToEdiVisual> {
        explicit ToEdiVisual() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;

    struct ToEdiVisualLine : public BindedFuncCreator<ToEdiVisualLine> {
        explicit ToEdiVisualLine() ;
        static void sprocess(bool vclmodeout=true) ;
        static void sprocess(NTypeLogger& parent_lgr) ;
        static void sprocess(const CharLogger& parent_lgr) ;
    } ;
}

#endif
