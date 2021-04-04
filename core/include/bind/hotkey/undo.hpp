#ifndef _UNDO_HPP
#define _UNDO_HPP

#include <memory>

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    class SCRedo : public BindedFuncWithCreator<SCRedo> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) const ;
        static const std::string sname() noexcept ;

        explicit SCRedo() ;
        virtual ~SCRedo() noexcept ;

        SCRedo(SCRedo&&) ;
        SCRedo& operator=(SCRedo&&) ;
        SCRedo(const SCRedo&)               = delete ;
        SCRedo& operator=(const SCRedo&)    = delete ;
    } ;


    struct SCUndo : public BindedFuncWithCreator<SCUndo> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) const ;
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
