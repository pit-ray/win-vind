#ifndef _OPTIONLIST_HPP
#define _OPTIONLIST_HPP

#include "option.hpp"

#include <vector>


namespace vind
{
    namespace opt
    {
        const std::vector<Option::SPtr>& all_global_options() ;
    }
}

#endif
