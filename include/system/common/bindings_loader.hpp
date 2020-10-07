#ifndef _BINDINGS_LOADER_HPP
#define _BINDINGS_LOADER_HPP

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <memory>
#include "key_matcher.hpp"
#include "mode_manager.hpp"

namespace BindingsLoader
{
    using vc_t = std::vector<unsigned char> ;
    using vvc_t = std::vector<vc_t> ;
    using vvvc_t = std::vector<vvc_t> ;
    using keymap_t = std::unordered_map<std::string, vvvc_t> ;

    void load_config() noexcept ;
    const keymap_t& get_bindings() noexcept ;
    const keymap_t& get_commands() noexcept ;

    const std::unordered_set<unsigned char>& get_unbinded_syskeys() noexcept ;

    using modematcher_t = std::array<KeyMatcher::shp_t, static_cast<int>(ModeManager::Mode::NUM)> ;
} ;

#endif
