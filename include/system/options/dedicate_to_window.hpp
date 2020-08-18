#ifndef _DEDICATE_TO_WINDOW_HPP
#define _DEDICATE_TO_WINDOW_HPP

#include "dynamic_option.hpp"

class Dedicate2Window : public DynamicOption
{
private:
    bool do_enable() const noexcept override ;
    bool do_disable() const noexcept override ;
    bool do_process() const override ;

public:
    const std::string name() const noexcept override ;
    static std::unique_ptr<DynamicOption> create() ;

    explicit Dedicate2Window() ;
    virtual ~Dedicate2Window() noexcept ;
    Dedicate2Window(Dedicate2Window&&) noexcept ;
    Dedicate2Window& operator=(Dedicate2Window&&) noexcept ;

    Dedicate2Window(const Dedicate2Window&) = delete ;
    Dedicate2Window& operator=(const Dedicate2Window&) = delete ;
} ;

#endif