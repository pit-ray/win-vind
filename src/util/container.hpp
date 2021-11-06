#ifndef _CONTAINER_HPP
#define _CONTAINER_HPP

#include <algorithm>
#include <vector>

namespace vind
{
    namespace util {
        template <typename T>
        inline void remove_deplication(T&& vec) {
            if(vec.empty()) return ;
            std::sort(vec.begin(), vec.end()) ;
            decltype(auto) itr = std::unique(vec.begin(), vec.end()) ;
            vec.erase(itr, vec.end()) ;
        }

        template <typename T, std::size_t N>
        inline constexpr std::size_t sizeof_array(T(&) [N]) noexcept {
            return N ;
        }

        template <typename T>
        inline void remove_from_top(std::vector<T>& v, std::size_t num_from_top) {
            if(v.empty()) return ;

            if(v.size() < num_from_top) {
                v.clear() ;
                return ;
            }
            v.erase(v.begin(), v.begin() + num_from_top) ;
        }

        template <typename T>
        inline void remove_from_back(std::vector<T>& v, std::size_t num_from_back) {
            if(v.empty()) return ;

            if(v.size() < num_from_back) {
                v.clear() ;
                return ;
            }
            v.erase(v.end() - num_from_back, v.end()) ;
        }
    }
}

#endif
