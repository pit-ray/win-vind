#ifndef _EC_HINTS_HPP
#define _EC_HINTS_HPP

#include <string>
#include <vector>

#include "key/keycode_def.hpp"

namespace vind
{
    using Hint = std::vector<KeyCode> ;

    void assign_identifier_hints(
            std::size_t target_count,
            std::vector<Hint>& hints) ;

    void convert_hints_to_strings(
            const std::vector<Hint>& hints,
            std::vector<std::string>& hint_strings) ;
}

#endif
