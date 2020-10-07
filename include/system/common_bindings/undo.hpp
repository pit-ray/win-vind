#ifndef _UNDO_HPP
#define _UNDO_HPP

#include <memory>

#include "binded_func_with_creator.hpp"

struct SCRedo : public BindedFuncWithCreator<SCRedo>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
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


struct SCUndo : public BindedFuncWithCreator<SCUndo>
{
    bool sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
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
