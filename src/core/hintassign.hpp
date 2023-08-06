#ifndef _HINTASSIGN_HPP
#define _HINTASSIGN_HPP

#include <string>
#include <vector>

#include "keycode.hpp"
#include "keycodedef.hpp"

namespace vind
{
    namespace core
    {
        using Hint = std::vector<KeyCode> ;

        void assign_identifier_hints(
                std::size_t target_count,
                std::vector<Hint>& hint_codes,
                std::vector<std::string>& hint_labels,
                const std::string& hintkeys="asdghklqwertyuiopzxcvbnmfj") ;
    }
}

#endif
