#ifndef _UNDO_HPP
#define _UNDO_HPP

#include <memory>

#include "binded_func_with_creator.hpp"

class SCRedo : public BindedFuncWithCreator<SCRedo>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit SCRedo() ;
    virtual ~SCRedo() noexcept ;

    SCRedo(SCRedo&&) ;
    SCRedo& operator=(SCRedo&&) ;
    SCRedo(const SCRedo&)               = delete ;
    SCRedo& operator=(const SCRedo&)    = delete ;
} ;


struct SCUndo : public BindedFuncWithCreator<SCUndo>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    void sprocess(const bool first_call, const unsigned int repeat_num, const KeyLogger* const parent_logger) const ;
    static const std::string sname() noexcept ;

    explicit SCUndo() ;
    virtual ~SCUndo() noexcept ;

    SCUndo(SCUndo&&) ;
    SCUndo& operator=(SCUndo&&) ;
    SCUndo(const SCUndo&)               = delete ;
    SCUndo& operator=(const SCUndo&)    = delete ;
} ;
#endif
