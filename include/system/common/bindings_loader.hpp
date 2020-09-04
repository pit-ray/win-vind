#ifndef _BINDINGS_LOADER_HPP
#define _BINDINGS_LOADER_HPP

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <memory>

class BindingsLoader
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    using vc_t = std::vector<unsigned char> ;
    using vvc_t = std::vector<vc_t> ;
    using vvvc_t = std::vector<vvc_t> ;
    using keymap_t = std::unordered_map<std::string, vvvc_t> ;

    explicit BindingsLoader(const std::string& filepath) ;
    virtual ~BindingsLoader() noexcept ;

    BindingsLoader(BindingsLoader&&) noexcept ;
    BindingsLoader& operator=(BindingsLoader&&) noexcept ;

    BindingsLoader(const BindingsLoader&) = delete ;
    BindingsLoader& operator=(const BindingsLoader&) = delete ;

    bool parse() noexcept ;
    const keymap_t& get_bindings() const noexcept ;
    const keymap_t& get_commands() const noexcept ;
    const std::unordered_set<unsigned char>& get_ignored_syskeys() const noexcept ;
} ;

#endif