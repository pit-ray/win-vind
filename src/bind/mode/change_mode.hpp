#ifndef _CHANGE_MODE_HPP
#define _CHANGE_MODE_HPP

#include "bind/bindedfunc.hpp"

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

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;

        struct ToResident : public BindedFuncVoid<ToResident> {
            explicit ToResident() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;

        struct ToGUIVisual : public BindedFuncVoid<ToGUIVisual> {
            explicit ToGUIVisual() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;

        struct ToEdiNormal : public BindedFuncVoid<ToEdiNormal> {
            explicit ToEdiNormal() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;

        struct ToInsert : public BindedFuncVoid<ToInsert> {
            explicit ToInsert() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;

        struct ToEdiVisual : public BindedFuncVoid<ToEdiVisual> {
            explicit ToEdiVisual() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;

        struct ToEdiVisualLine : public BindedFuncVoid<ToEdiVisualLine> {
            explicit ToEdiVisualLine() ;
            static void sprocess(
                std::uint16_t count,
                const std::string& args,
                bool vclmodeout=true) ;

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;
    }
}

#endif
