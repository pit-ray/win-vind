#ifndef _BINDINGS_JSON_PARSER_HPP
#define _BINDINGS_JSON_PARSER_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

class BindingsJSONParser
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

    void init() noexcept ;

public:
    using vc_t = std::vector<unsigned char> ;
    using vvc_t = std::vector<vc_t> ;
    using vvvc_t = std::vector<vvc_t> ;
    using keymap_t = std::unordered_map<std::string, vvvc_t> ;

    explicit BindingsJSONParser(const std::string& filepath) ;
    virtual ~BindingsJSONParser() noexcept ;

    BindingsJSONParser(BindingsJSONParser&&) noexcept ;
    BindingsJSONParser& operator=(BindingsJSONParser&&) noexcept ;

    BindingsJSONParser(const BindingsJSONParser&) = delete ;
    BindingsJSONParser& operator=(const BindingsJSONParser&) = delete ;

    const keymap_t& get_bindings() const noexcept ;
    const keymap_t& get_commands() const noexcept ;
    const vc_t& get_ignored_syskeys() const noexcept ;
} ;

#endif