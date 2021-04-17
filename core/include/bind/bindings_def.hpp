#ifndef _BINDINGS_DEF_HPP
#define _BINDINGS_DEF_HPP

#include "bind/base/mode.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace vind
{
    using KeySet = std::vector<unsigned char> ;
    using Command = std::vector<KeySet> ;
    using CommandList = std::vector<Command> ;

    using ParsedStringCmd = std::vector<std::string> ;

    template <typename T>
    using ModeArray = std::array<T, mode::mode_num()> ;

    template <typename T>
    using FuncMap = std::unordered_map<std::string, T> ;

    using ParsedStringBindingLists = FuncMap<ModeArray<ParsedStringCmd>> ;

    using ParsedBindingLists = ModeArray<FuncMap<std::shared_ptr<CommandList>>> ;
}

#endif
