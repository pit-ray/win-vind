#ifndef _COREIO_FAKE_PATH_HPP
#define _COREIO_FAKE_PATH_HPP

#include "path.hpp"

namespace vind
{
    namespace path {
        const std::filesystem::path& ROOT_PATH() {
            static std::filesystem::path p{} ;
            return p ;
        }
    }
}

#endif
