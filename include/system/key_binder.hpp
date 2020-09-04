#ifndef _KEY_BINDER_HPP
#define _KEY_BINDER_HPP

#include <memory>
#include <string>
#include <vector>

#include "key_binding.hpp"
#include "command.hpp"

class KeyBinder
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

    void update_core() noexcept ;
    void update_core_cmd() noexcept ;

public:
    explicit KeyBinder() ;
    virtual ~KeyBinder() noexcept ;

    void load_config() noexcept ;
    void update() noexcept ;

    const std::string get_logger_str() const noexcept ;

    KeyBinder(KeyBinder&&) = delete ;
    KeyBinder& operator=(KeyBinder&&) = delete ;
    KeyBinder(const KeyBinder&) = delete ;
    KeyBinder& operator=(const KeyBinder&) = delete ;
} ;

#endif