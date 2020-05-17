#ifndef _WINDOW_CTRL_HPP
#define _WINDOW_CTRL_HPP

#include "binded_function_with_creator.hpp"
#include "command_with_creator.hpp"

struct CloseCurrentWindow
: public BindedFunctionWithCreator<CloseCurrentWindow>,
  public CommandWithCreator<CloseCurrentWindow>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string& cmd) ;
    static const std::string sname() noexcept ;

private:
    static bool common_process() ;
} ;

struct SwitchWindow
: public BindedFunctionWithCreator<SwitchWindow>,
  public CommandWithCreator<SwitchWindow>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string& cmd) ;
    static const std::string sname() noexcept ;

private:
    static bool common_process() ;
} ;

struct MaximizeCurrentWindow
: public BindedFunctionWithCreator<MaximizeCurrentWindow>,
  public CommandWithCreator<MaximizeCurrentWindow>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string& cmd) ;
    static const std::string sname() noexcept ;

private:
    static bool common_process() ;
} ;

struct MinimizeCurrentWindow
: public BindedFunctionWithCreator<MinimizeCurrentWindow>,
  public CommandWithCreator<MinimizeCurrentWindow>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string& cmd) ;
    static const std::string sname() noexcept ;

private:
    static bool common_process() ;
} ;

struct SnapCurrentWindow2Left
: public BindedFunctionWithCreator<SnapCurrentWindow2Left>,
  public CommandWithCreator<SnapCurrentWindow2Left>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string& cmd) ;
    static const std::string sname() noexcept ;

private:
    static bool common_process() ;
} ;

struct SnapCurrentWindow2Right
: public BindedFunctionWithCreator<SnapCurrentWindow2Right>,
  public CommandWithCreator<SnapCurrentWindow2Right>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string& cmd) ;
    static const std::string sname() noexcept ;

private:
    static bool common_process() ;
} ;
#endif