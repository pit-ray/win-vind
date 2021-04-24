#ifndef _EC_HINTS_HPP
#define _EC_HINTS_HPP

#include <string>
#include <vector>

namespace vind
{
    namespace easyclick {
        using Hint = std::vector<unsigned char> ;

        std::vector<Hint> assign_identifier_hints(std::size_t target_count) ;

        std::vector<std::string> convert_hints_to_strings(const std::vector<Hint>& hints) ;
    }
}

#endif
