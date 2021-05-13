#ifndef _COREIO_FAKE_PATH_HPP
#define _COREIO_FAKE_PATH_HPP

#include "path.hpp"

namespace vind
{
    namespace path {
        const std::string& ROOT_PATH() {
            static std::string s{} ;
            return s ;
        }
    }
}

#endif
