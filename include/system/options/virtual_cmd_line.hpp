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

    bool do_enable() const noexcept override ;
    bool do_disable() const noexcept override ;
    bool do_process() const override ;

public:
    explicit VirtualCmdLine() ;

    static const std::string sname() noexcept ;

    static void cout(std::string&& str) noexcept ;
    static void cout(const std::string& str) noexcept ;
    static void msgout(std::string str) noexcept ;

    static void refresh() noexcept ;
    static void clear() noexcept ;
    static void reset() noexcept ;

    virtual ~VirtualCmdLine() noexcept ;
    VirtualCmdLine(VirtualCmdLine&&) noexcept ;
    VirtualCmdLine& operator=(VirtualCmdLine&&) noexcept ;

    VirtualCmdLine(const VirtualCmdLine&) = delete ;
    VirtualCmdLine& operator=(const VirtualCmdLine&) = delete ;
} ;
#endif
