#ifndef _KEY_LOGGER_HPP
#define _KEY_LOGGER_HPP

#include <vector>
#include "key_log.hpp"

using KeyLogger = std::vector<KeyLog> ;

namespace KyLgr
{
    const std::string log2str(const KeyLogger& lgr) ;
    unsigned int extract_from_str(const std::string str) ;

    //[true]  logger is changed
    //[false] logger is not changed
    bool log_as_vkc(KeyLogger& lgr) ; //logging direct
    bool log_as_char(KeyLogger& lgr) ; //regard inputed key log as ascii charactor
}

#endif
