#ifndef _COMMAND_MODE_HPP
#define _COMMAND_MODE_HPP

#include "binded_func_with_creator.hpp"

class CommandMode : public BindedFuncWithCreator<CommandMode>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    explicit CommandMode() ;
    static const std::string sname() noexcept ;
    void sprocess(
            const bool first_call,
            const unsigned int repeat_num,
            VKCLogger* const parent_vkclgr,
            const CharLogger* const parent_charlgr) const ;

    virtual ~CommandMode() noexcept ;
    CommandMode(CommandMode&&) ;
    CommandMode& operator=(CommandMode&&) ;
    CommandMode(const CommandMode&)            = delete ;
    CommandMode& operator=(const CommandMode&) = delete ;
} ;

#endif
