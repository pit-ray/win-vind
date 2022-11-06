#ifndef _CHANGE_MODE_HPP
#define _CHANGE_MODE_HPP

#include "bind/bindedfunc.hpp"
#include "text_area_scanner.hpp"

namespace vind
{
    namespace bind
    {
        struct ToGUINormal : public BindedFuncVoid<ToGUINormal> {
            explicit ToGUINormal() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;
        } ;

        struct ToResident : public BindedFuncVoid<ToResident> {
            explicit ToResident() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;
        } ;

        struct ToGUIVisual : public BindedFuncVoid<ToGUIVisual> {
            explicit ToGUIVisual() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;
        } ;

        class ToEdiNormal : public BindedFuncVoid<ToEdiNormal> {
        private:
            static TextAreaScanner scanner_ ;

        public:
            explicit ToEdiNormal() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;
        } ;

        struct ToInsert : public BindedFuncVoid<ToInsert> {
            explicit ToInsert() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;
        } ;

        struct ToEdiVisual : public BindedFuncVoid<ToEdiVisual> {
            explicit ToEdiVisual() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;
        } ;

        struct ToEdiVisualLine : public BindedFuncVoid<ToEdiVisualLine> {
            explicit ToEdiVisualLine() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;
        } ;
    }
}

#endif
