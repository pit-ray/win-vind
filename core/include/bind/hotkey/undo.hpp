#ifndef _UNDO_HPP
#define _UNDO_HPP

#include <memory>

#include "bind/binded_func_creator.hpp"

namespace vind
{
    class SCRedo : public BindedFuncCreator<SCRedo> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit SCRedo() ;
        virtual ~SCRedo() noexcept ;

        SCRedo(SCRedo&&) ;
        SCRedo& operator=(SCRedo&&) ;
        SCRedo(const SCRedo&)               = delete ;
        SCRedo& operator=(const SCRedo&)    = delete ;
    } ;


    struct SCUndo : public BindedFuncCreator<SCUndo> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit SCUndo() ;
        virtual ~SCUndo() noexcept ;

        SCUndo(SCUndo&&) ;
        SCUndo& operator=(SCUndo&&) ;
        SCUndo(const SCUndo&)               = delete ;
        SCUndo& operator=(const SCUndo&)    = delete ;
    } ;
}
#endif
