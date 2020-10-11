#ifndef _VIRTUAL_CMD_LINE_HPP
#define _VIRTUAL_CMD_LINE_HPP

#include "dynamic_option_with_creator.hpp"
#include <chrono>

class VirtualCmdLine : public DynamicOptionWithCreator<VirtualCmdLine>
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

    static std::string outstr ;
    static std::chrono::system_clock::time_point msg_start ;
    static bool msg_showing ;

    void do_enable() const override ;
    void do_disable() const override ;
    void do_process() const override ;

public:
    explicit VirtualCmdLine() ;

    static const std::string sname() noexcept ;

    static void cout(std::string&& str) noexcept ;
    static void cout(const std::string& str) noexcept ;
    static void msgout(std::string str) noexcept ;

    static void refresh() ;
    static void clear() noexcept ;
    static void reset() noexcept ;

    virtual ~VirtualCmdLine() noexcept ;
    VirtualCmdLine(VirtualCmdLine&&) ;
    VirtualCmdLine& operator=(VirtualCmdLine&&) ;
    VirtualCmdLine(const VirtualCmdLine&)            = delete ;
    VirtualCmdLine& operator=(const VirtualCmdLine&) = delete ;
} ;
#endif
