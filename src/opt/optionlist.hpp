#ifndef _OPTIONLIST_HPP
#define _OPTIONLIST_HPP

#include "option.hpp"

#include <initializer_list>
#include <vector>


namespace vind
{
    namespace opt
    {
        const std::vector<Option::SPtr>& all_global_options() ;

        Option::SPtr search_option(const std::string& name) ;
        Option::SPtr search_option(std::string&& name) ;

        template <typename String, typename... Strings>
        std::vector<Option::SPtr> search_options(String&& name, Strings&&... names) {
            std::vector<Option::SPtr> out ;

            if(auto opt = search_option(std::forward<String>(name))) {
                out.push_back(std::move(opt)) ;
            }

            auto append = [&out](auto&& ptr) {
                if(ptr) {
                    out.push_back(std::forward<decltype(ptr)>(ptr)) ;
                }
            } ;

            ((append(search_option(std::forward<String>(names)))), ...) ;

            return out ;
        }

        template <typename T>
        inline bool check_if_option(T&& name) {
            return search_option(name) != nullptr ;
        }
    }
}

#endif
