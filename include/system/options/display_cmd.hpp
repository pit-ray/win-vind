#ifndef _DISPLAY_CMD_HPP
#define _DISPLAY_CMD_HPP

#include "dynamic_option.hpp"

class KeyBinder ;

class DisplayCmd : public DynamicOption
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
    bool do_enable() const noexcept override ;
    bool do_disable() const noexcept override ;
    bool do_process() const override ;

public:
    const std::string name() const noexcept override ;

    static std::unique_ptr<DynamicOption> create(const KeyBinder* const pkb) ;

    explicit DisplayCmd(const KeyBinder* const pkb) ;
    virtual ~DisplayCmd() noexcept ;
    DisplayCmd(DisplayCmd&&) noexcept ;
    DisplayCmd& operator=(DisplayCmd&&) noexcept ;

    DisplayCmd(const DisplayCmd&) = delete ;
    DisplayCmd& operator=(const DisplayCmd&) = delete ;
} ;
#endif