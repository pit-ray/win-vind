#ifndef _MAP_DEFAULT_HPP
#define _MAP_DEFAULT_HPP

#include "core/mode.hpp"

#include <string>
#include <utility>
#include <vector>


namespace vind
{
    namespace core
    {
        const std::vector<std::pair<std::string, std::string>>& get_default_map(Mode mode) ;

        template <typename T>
        inline const std::vector<std::pair<std::string, std::string>>& get_default_map(T mode) {
            return get_default_map(static_cast<Mode>(mode)) ;
        }
    }
}

#endif
