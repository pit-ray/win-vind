#ifndef _UNDO_HPP
#define _UNDO_HPP

#include <memory>

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        class Redo : public BindedFuncVoid<Redo> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit Redo() ;
            virtual ~Redo() noexcept ;

            Redo(Redo&&) ;
            Redo& operator=(Redo&&) ;
            Redo(const Redo&)               = delete ;
            Redo& operator=(const Redo&)    = delete ;
        } ;


        struct Undo : public BindedFuncVoid<Undo> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit Undo() ;
            virtual ~Undo() noexcept ;

            Undo(Undo&&) ;
            Undo& operator=(Undo&&) ;
            Undo(const Undo&)               = delete ;
            Undo& operator=(const Undo&)    = delete ;
        } ;
    }
}
#endif
