#ifndef _KEY_BINDER_HPP
#define _KEY_BINDER_HPP

#include <memory>
#include <string>
#include <vector>

#include "binded_function.hpp"
#include "command.hpp"

class KeyBinder
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

    void update_core(const std::vector<bf::shp_t>& vp) noexcept ;
    void update_core_cmd(const std::vector<cmd::shp_t>& vp) noexcept ;

public:
    explicit KeyBinder() ;
    explicit KeyBinder(const std::string& filename) ;
    ~KeyBinder() noexcept ;

    void load_config(const std::string& filename) noexcept ;
    void update() noexcept ;

    const std::string get_logger_str() const noexcept ;

    KeyBinder(KeyBinder&&) = delete ;
    KeyBinder& operator=(KeyBinder&&) = delete ;
    KeyBinder(const KeyBinder&) = delete ;
    KeyBinder& operator=(const KeyBinder&) = delete ;
} ;

#endif