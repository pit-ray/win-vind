#ifndef _DEDICATE_TO_WINDOW_CTRL_HPP
#define _DEDICATE_TO_WINDOW_CTRL_HPP

#include "key_binding_with_creator.hpp"
#include "command_with_creator.hpp"

class EnableTargetingOfDedicate2Window
: public KeyBindingWithCreator<EnableTargetingOfDedicate2Window>,
  public CommandWithCreator<EnableTargetingOfDedicate2Window>
{
private:
    static bool common_process() ;

public:
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;
} ;


class DisableTargetingOfDedicate2Window
: public KeyBindingWithCreator<DisableTargetingOfDedicate2Window>,
  public CommandWithCreator<DisableTargetingOfDedicate2Window>
{
private:
    static bool common_process() ;

public:
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;
} ;
#endif
