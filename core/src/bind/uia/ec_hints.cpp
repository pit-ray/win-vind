#include "bind/uia/ec_hints.hpp"

#include "key/keycode_def.hpp"
#include "key/keycodecvt.hpp"

#include <array>
#include <stdexcept>
#include <vector>

namespace
{
    using namespace vind ;
    constexpr std::array<KeyCode, 26> gcx_labels = {
        KEYCODE_A, KEYCODE_S, KEYCODE_D, KEYCODE_G, KEYCODE_H,
        KEYCODE_K, KEYCODE_L, KEYCODE_Q, KEYCODE_W, KEYCODE_E,
        KEYCODE_R, KEYCODE_T, KEYCODE_Y, KEYCODE_U, KEYCODE_I,
        KEYCODE_O, KEYCODE_P, KEYCODE_Z, KEYCODE_X, KEYCODE_C,
        KEYCODE_V, KEYCODE_B, KEYCODE_N, KEYCODE_M, KEYCODE_F, KEYCODE_J
    } ;
}

namespace vind
{
    //Currrently, supported only 26 x 26 x 26 = 17576 patterns.
    void assign_identifier_hints(std::size_t target_count, std::vector<Hint>& hints) {
        // <= 26
        if(target_count <= gcx_labels.size()) {
            hints.resize(target_count) ;
            for(std::size_t i = 0 ; i < target_count ; i ++) {
                hints[i].push_back(gcx_labels[i]) ;
            }
            return ;
        }

        // <= 26 * 26 (=676)
        constexpr auto gcx_labels_size_pow2 = gcx_labels.size() * gcx_labels.size() ;
        if(target_count <= gcx_labels_size_pow2) {
            hints.resize(target_count) ;

            std::size_t idx = 0 ;
            auto l2_num = target_count / gcx_labels.size() + 1 ;
            for(auto i = l2_num ; i < gcx_labels.size() ; i ++) {
                hints[idx].push_back(gcx_labels[i]) ;
                idx ++ ;
            }

            for(std::size_t j = 0 ; j < l2_num ; j ++) {
                try {
                    for(std::size_t  i = 0 ; i < gcx_labels.size() ; i ++) {
                        hints.at(idx).push_back(gcx_labels[j]) ;
                        hints[idx].push_back(gcx_labels[i]) ;
                        idx ++ ;
                    }
                }
                catch(const std::out_of_range&) {break ;}
            }

            return ;
        }

        // <= 26 * 26 * 26 (=17576)
        static constexpr auto gcx_labels_size_pow3 = gcx_labels_size_pow2  * gcx_labels.size() ;
        if(target_count <= gcx_labels_size_pow3) {
            hints.resize(target_count) ;
        } else {
            hints.resize(gcx_labels_size_pow3) ;
        }
        auto l3_num = target_count / gcx_labels_size_pow2 + 1 ;

        std::size_t idx = 0 ;
        for(std::size_t j = l3_num ; j < gcx_labels.size() ; j ++) {
            for(std::size_t i = 0 ; i < gcx_labels.size() ; i ++) {
                hints[idx].push_back(gcx_labels[j]) ;
                hints[idx].push_back(gcx_labels[i]) ;
                idx ++ ;
            }
        }

        for(std::size_t k = 0 ; k < l3_num ; k ++) {
            try {
                for(std::size_t j = 0 ; j < gcx_labels.size() ; j ++) {
                    for(std::size_t i = 0 ; i < gcx_labels.size() ; i ++) {
                        hints.at(idx).push_back(gcx_labels[k]) ;
                        hints[idx].push_back(gcx_labels[j]) ;
                        hints[idx].push_back(gcx_labels[i]) ;
                        idx ++ ;
                    }
                }
            }
            catch(const std::out_of_range&) {break ;}
        }
    }

    void convert_hints_to_strings(const std::vector<Hint>& hints, std::vector<std::string>& hint_strings) {
        hint_strings.resize(hints.size()) ;

        for(std::size_t i = 0 ; i < hints.size() ; i ++) {
            std::string str ;
            for(auto& key : hints[i]) {
                str.push_back(keycodecvt::get_shifted_ascii(key)) ;
            }
            hint_strings[i] = std::move(str) ;
        }
    }
}
