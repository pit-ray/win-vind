#ifndef _KEY_BINDING_HPP
#define _KEY_BINDING_HPP

#include <memory>
#include <string>

#include "bindings_loader.hpp"
#include "key_logger.hpp"

class KeyBinding ;

namespace kbg
{
    using shp_t = std::shared_ptr<KeyBinding> ;
}

class KeyLog ;

//base
class KeyBinding
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

    virtual bool do_process(const bool first_call) const = 0 ;

public:
    explicit KeyBinding() ;
    virtual ~KeyBinding() noexcept ;

    KeyBinding(KeyBinding&&) noexcept ;
    KeyBinding& operator=(KeyBinding&&) noexcept ;

    KeyBinding(const KeyBinding&) = delete ;
    KeyBinding& operator=(const KeyBinding&) = delete ;

    virtual const std::string name() const noexcept = 0 ;

    void process(const bool first_call=true) const ;

    void set_command(const BindingsLoader::vvvc_t& command) noexcept ;

    std::size_t matched_num(const KeyLog& log, const std::size_t seq_index=0) const noexcept ;

    bool is_callable() const noexcept ;
} ;

#endif
