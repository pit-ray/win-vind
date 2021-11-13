#ifndef _UNDO_HPP
#define _UNDO_HPP

#include <memory>

#include "bind/binded_func_creator.hpp"

namespace vind
{
    class Redo : public BindedFuncCreator<Redo> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(core::NTypeLogger& parent_lgr) const ;
        void sprocess(const core::CharLogger& parent_lgr) const ;

        explicit Redo() ;
        virtual ~Redo() noexcept ;

        Redo(Redo&&) ;
        Redo& operator=(Redo&&) ;
        Redo(const Redo&)               = delete ;
        Redo& operator=(const Redo&)    = delete ;
    } ;


    struct Undo : public BindedFuncCreator<Undo> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(core::NTypeLogger& parent_lgr) const ;
        void sprocess(const core::CharLogger& parent_lgr) const ;

        explicit Undo() ;
        virtual ~Undo() noexcept ;

        Undo(Undo&&) ;
        Undo& operator=(Undo&&) ;
        Undo(const Undo&)               = delete ;
        Undo& operator=(const Undo&)    = delete ;
    } ;
}
#endif
