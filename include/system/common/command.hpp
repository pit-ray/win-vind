#ifndef _COMMAND_HPP
#define _COMMAND_HPP
#include <memory>
#include <string>

#include "bindings_loader.hpp"

class Command ;

namespace cmd
{
    using shp_t = std::shared_ptr<Command> ;
}

class KeyLog ;
class KeyLogger ;

//base
class Command
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

    virtual bool do_process(const std::string cmd) const = 0 ;

public:
    explicit Command() ;
    virtual ~Command() noexcept ;

    Command(Command&&) noexcept ;
    Command& operator=(Command&&) noexcept ;

    Command(const Command&) = delete ;
    Command& operator=(const Command&) = delete ;

    virtual const std::string name() const noexcept = 0 ;

    //command may be short, so will apply SSO.
    void process(const std::string cmd) const ;

    void set_command(const BindingsLoader::vvvc_t& command) noexcept ;

    bool is_callable(const KeyLogger& logger) const noexcept ;
} ;

#endif
