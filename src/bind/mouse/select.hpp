#ifndef _SELECT_HPP
#define _SELECT_HPP
#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct SelectAll : public BindedFuncVoid<SelectAll> {
            explicit SelectAll() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct ForwardUINavigation : public BindedFuncVoid<ForwardUINavigation> {
            explicit ForwardUINavigation() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct BackwardUINavigation : public BindedFuncVoid<BackwardUINavigation> {
            explicit BackwardUINavigation() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct DecideFocusedUIObject : public BindedFuncVoid<DecideFocusedUIObject> {
            explicit DecideFocusedUIObject() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
