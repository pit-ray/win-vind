#include "coreio/path.hpp"

namespace vind
{
    namespace path {
        const std::string& ROOT_PATH() {
            static std::string s{} ;
            return s ;
        }
    }
}
