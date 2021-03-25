#ifndef _BIND_HPP
#define _BIND_HPP

#include "binded_func.hpp"
#include "mode.hpp"

namespace vind
{
    class KeyLoggerBase ;

    namespace keybind
    {
        void init() ;

        enum class InvalidPolicy : unsigned char {
            None,
            AllSystemKey,
            UnbindedSystemKey
        } ;

        bool is_invalid_log(
                const KeyLog& log,
                const InvalidPolicy ip=InvalidPolicy::AllSystemKey) ;

        const BindedFunc::shp_t find_func(
                const KeyLoggerBase& lgr,
                const BindedFunc::shp_t& running_func=nullptr,
                const bool full_scan=false, //The cost for computing is high.
                mode::Mode mode=mode::get_mode()) ;

        const BindedFunc::shp_t find_func_byname(const std::string& name) ;

        void load_config() ;
        void call_matched_funcs() ;
    }
}

#endif
