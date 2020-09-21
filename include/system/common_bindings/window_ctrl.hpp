#ifndef _WINDOW_CTRL_HPP
#define _WINDOW_CTRL_HPP

#include "key_binding_with_creator.hpp"
#include "command_with_creator.hpp"

struct CloseCurrentWindow
: public KeyBindingWithCreator<CloseCurrentWindow>,
  public CommandWithCreator<CloseCurrentWindow>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;

private:
    static bool common_process() ;
} ;

struct SwitchWindow
: public KeyBindingWithCreator<SwitchWindow>,
  public CommandWithCreator<SwitchWindow>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;

private:
    static bool common_process() ;
} ;

struct MaximizeCurrentWindow
: public KeyBindingWithCreator<MaximizeCurrentWindow>,
  public CommandWithCreator<MaximizeCurrentWindow>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;

private:
    static bool common_process() ;
} ;

struct MinimizeCurrentWindow
: public KeyBindingWithCreator<MinimizeCurrentWindow>,
  public CommandWithCreator<MinimizeCurrentWindow>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;

private:
    static bool common_process() ;
} ;

struct SnapCurrentWindow2Left
: public KeyBindingWithCreator<SnapCurrentWindow2Left>,
  public CommandWithCreator<SnapCurrentWindow2Left>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;

private:
    static bool common_process() ;
} ;

struct SnapCurrentWindow2Right
: public KeyBindingWithCreator<SnapCurrentWindow2Right>,
  public CommandWithCreator<SnapCurrentWindow2Right>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;

private:
    static bool common_process() ;
} ;

struct OpenNewCurrentWindow
: public KeyBindingWithCreator<OpenNewCurrentWindow>,
  public CommandWithCreator<OpenNewCurrentWindow>
{
    static bool sprocess(const bool first_call) ;
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;

private:
    static bool common_process() ;
} ;

struct ReloadCurrentWindow
: public CommandWithCreator<ReloadCurrentWindow>
{
    static bool sprocess(const std::string cmd) ;
    static const std::string sname() noexcept ;
} ;

struct Switch2LeftTab
: public KeyBindingWithCreator<Switch2LeftTab>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;

struct Switch2RightTab
: public KeyBindingWithCreator<Switch2RightTab>
{
    static bool sprocess(const bool first_call) ;
    static const std::string sname() noexcept ;
} ;
#endif
