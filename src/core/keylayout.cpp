#include "keylayout.hpp"

#include "langid.hpp"
#include "path.hpp"

namespace vind
{
    namespace core {
        std::filesystem::path get_keylayout_filepath(LANGID id) {

            std::string filename ;
            switch(id) {
                case LANGID_EN:
                case LANGID_EN_US:
                    filename = "US.kmp" ;
                    break ;

                case LANGID_JA:
                case LANGID_JA_JP:
                case LANGID_JA_PLOC_JP:
                    filename = "JP.kmp" ;
                    break ;

                default:
                    filename = "US.kmp" ;
                    break ;
            }

            return DEFAULT_CONFIG_PATH() / filename ;
        }
    }
}
