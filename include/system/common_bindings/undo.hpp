#ifndef _UNDO_HPP
#define _UNDO_HPP

#include <memory>

#include "key_binding_with_creator.hpp"

struct SCRedo : public KeyBindingWithCreator<SCRedo>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit SCRedo() ;
    virtual ~SCRedo() noexcept ;

    SCRedo(SCRedo&&) noexcept ;
    SCRedo& operator=(SCRedo&&) noexcept ;

    SCRedo(const SCRedo&) = delete ;
    SCRedo& operator=(const SCRedo&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;


struct SCUndo : public KeyBindingWithCreator<SCUndo>
{
    bool sprocess(const bool first_call) const ;
    static const std::string sname() noexcept ;

    explicit SCUndo() ;
    virtual ~SCUndo() noexcept ;

    SCUndo(SCUndo&&) noexcept ;
    SCUndo& operator=(SCUndo&&) noexcept ;

    SCUndo(const SCUndo&) = delete ;
    SCUndo& operator=(const SCUndo&) = delete ;

private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
} ;
#endif
