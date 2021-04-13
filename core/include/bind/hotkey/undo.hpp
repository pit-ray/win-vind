#ifndef _UNDO_HPP
#define _UNDO_HPP

#include <memory>

#include "bind/base/binded_func_with_creator.hpp"

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
        static const std::string sname() noexcept ;

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
        static const std::string sname() noexcept ;

        explicit SCUndo() ;
        virtual ~SCUndo() noexcept ;

        SCUndo(SCUndo&&) ;
        SCUndo& operator=(SCUndo&&) ;
        SCUndo(const SCUndo&)               = delete ;
        SCUndo& operator=(const SCUndo&)    = delete ;
    } ;
}
#endif
