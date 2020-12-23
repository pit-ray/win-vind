#include "key_logger.hpp"

#include <iostream>
#include <stdexcept>

#include "key_absorber.hpp"
#include "keystroke_repeater.hpp"
#include "utility.hpp"
#include "virtual_key_fwd.hpp"
#include "vkc_converter.hpp"

using namespace std ;

namespace KyLgr {
    const std::string log2str(const KeyLogger& lgr)
    {
        if(lgr.empty()) return "" ;

        string str{} ;
        for(const auto& log : lgr) {
            if(log.is_containing(VKC_SHIFT)) {
                //shifted ascii
                for(const auto vkc : log) {
                    const auto c = VKCConverter::get_shifted_ascii(vkc) ;
                    if(c != 0) str.push_back(c) ;
                }
                continue ;
            }
            for(const auto vkc : log) {
                const auto c = VKCConverter::get_ascii(vkc) ;
                if(c != 0) str.push_back(c) ;
            }
        }
        return str ;
    }

    unsigned int extract_from_str(const std::string str) {
        static constexpr auto c_nums = "0123456789" ;

        auto bpos = str.find_first_of(c_nums) ;
        if(bpos == std::string::npos) return 0 ;

        auto epos = str.find_first_not_of(c_nums, bpos) ;

        return std::stoi(str.substr(bpos, epos)) ;
    }

    bool log_as_vkc(KeyLogger& lgr)
    {
        static KeyLog prelog{} ;

        auto log = KeyAbsorber::get_pressed_list() ;
        lgr.push_back(log) ;

        if(prelog == log) {
            return false ;
        }

        if(lgr.size() == 1) {
            prelog = log ;
            return true ;
        }

        auto diff = log - prelog ; //remove same keys as prelog
        prelog = log ;
        return !diff.empty() ;
        //
        // If the number of logs in logger is bigger than 1,
        // some functions are matched, so save only differences.
        //
        // Ex)
        //  _______________________________________________________
        // |              |          |        |          |         |
        // | logger-index |   [1]    |  [2]   |    [3]   |   [4]   |
        // |--------------|----------|--------|----------|---------|
        // |     log      | Ctrl + w |  Ctrl  |          |    L    |
        // |--------------|----------|--------|----------|---------|
        // |    return    |   true   | false  |   false  |   true  |
        // |______________|__________|________|__________|_________|
        //
    }

    bool log_as_char(KeyLogger& lgr)
    {
        static KeyLog prelog{} ;
        static bool changed = true ;
        static KeyStrokeRepeater ksr{} ;

        auto log = KeyAbsorber::get_pressed_list() ;

        if(log != prelog) { //type is changed
            changed = true ;

            const auto diff = log - prelog ;
            prelog = log ;

            if(!log.is_containing(VKC_SHIFT)) {
                lgr.push_back(std::move(diff)) ;
                return true ;
            }

            //shfited
            auto data = diff.get() ;
            data.insert(VKC_SHIFT) ;

            //construct KeyLog inside logs directly from std::vector
            lgr.emplace_back(std::move(data)) ;
            return true ;
        }
        else { //long pressing
            lgr.push_back(log) ;
            if(changed) {
                changed = false ;
                ksr.reset() ;
                return false ;
            }
            return ksr.is_pressed() ; //emulate key stroke
        }
    }
}
