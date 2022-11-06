#ifndef _UNDO_HPP
#define _UNDO_HPP

#include <memory>

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        struct Redo : public BindedFuncVoid<Redo> {
            explicit Redo() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct Undo : public BindedFuncVoid<Undo> {
            explicit Undo() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}
#endif
