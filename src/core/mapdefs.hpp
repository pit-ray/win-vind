#ifndef _MAPDEFS_HPP
#define _MAPDEFS_HPP

#include "keycode_def.hpp"
#include "mode.hpp"

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>

namespace vind
{
    using KeySet = std::vector<KeyCode> ;
    using Command = std::vector<KeySet> ;
    using CommandList = std::vector<Command> ;

    using ParsedStringCmd = std::vector<std::string> ;

    template <typename T>
    using ModeArray = std::array<T, core::mode_num()> ;

    template <typename T>
    using FuncMap = std::unordered_map<std::string, T> ;

    using ParsedStringBindingLists = FuncMap<ModeArray<ParsedStringCmd>> ;

    using ParsedBindingLists = ModeArray<FuncMap<std::shared_ptr<CommandList>>> ;

    constexpr auto MAP_DEFINE_KEYWORD_IN_JSON = "def" ;
}

#endif
