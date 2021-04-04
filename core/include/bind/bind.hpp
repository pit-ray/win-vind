#ifndef _BIND_HPP
#define _BIND_HPP

#include "binded_func.hpp"
#include "mode.hpp"

namespace vind
{
    class KeyLoggerBase ;

    namespace keybind
    {
        void initialize() ;

        enum class InvalidPolicy : unsigned char {
            None,
            AllSystemKey,
            UnbindedSystemKey
        } ;

        bool is_invalid_log(
                const KeyLog& log,
                const InvalidPolicy ip=InvalidPolicy::AllSystemKey) ;

        //
        // It finds a function in the function list with the key logger.
        //
        // low_priority_func : regards this function as a low priority to choose a newer function.
        //                     If inputted `k` and `j`, each function is the same priority in default, so undefined which function is chosen.
        //
        // full_scan         : It enforces matching from the beginning of the logger.
        //                     In default, BindingsMatcher matching like Automata, so each function compares once.
        //                     If you full_scan=true, BindingsMatcher will not match sequentially, but match all logs by comparing the same times as logger.size().
        //
        const BindedFunc::shp_t find_func(
                const KeyLoggerBase& lgr,
                const BindedFunc::shp_t& low_priority_func=nullptr,
                const bool full_scan=false, //The cost for computing is high.
                mode::Mode mode=mode::get_global_mode()) ;

        const BindedFunc::shp_t find_func_byname(const std::string& name) ;

        void load_config() ;
        void call_matched_funcs() ;


        // It extracts a number from the log.
        // Ex)
        //      log     :   {KEYCODE_A, KEYCODE_1, KEYCODE_2}
        //      return  :   1
        unsigned int extract_number_keycode(const KeyLog& log) ;

        // It concatenates a number keycode to the number.
        // Ex)
        //      number      :   3
        //      num_keycode :   KEYCODE_1
        //
        //      number -> 31
        void concatenate_keycode_as_number(
                unsigned int& number,
                unsigned char num_keycode) ;
    }
}

#endif
