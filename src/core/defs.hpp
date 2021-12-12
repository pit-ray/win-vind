#ifndef _DEFS_HPP
#define _DEFS_HPP

#include "keycode_def.hpp"
#include "mode.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>


namespace vind
{
    enum class SystemCall : unsigned char {
        NOTHING     = 0b0000'0000,
        TERMINATE   = 0b0000'0001,
        RECONSTRUCT = 0b0000'0010
    } ;

    using KeySet = std::vector<KeyCode> ;
    using Command = std::vector<KeySet> ;
    using CommandList = std::vector<Command> ;

    using ParsedStringCmd = std::vector<std::string> ;

    template <typename T>
    using ModeArray = std::array<T, core::mode_num()> ;

    template <typename T>
    using FuncMap = std::unordered_map<std::string, T> ;

    using ParsedStringBindingLists = FuncMap<ModeArray<ParsedStringCmd>> ;

    using ParsedBindingTable = ModeArray<FuncMap<std::shared_ptr<CommandList>>> ;

    constexpr auto MAP_DEFINE_KEYWORD_IN_JSON = "def" ;
}

#endif
