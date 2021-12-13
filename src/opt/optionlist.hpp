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

        Option::SPtr ref_global_options_bynames(const std::string& name) ;
        Option::SPtr ref_global_options_bynames(std::string&& name) ;

        template <typename String, typename... Strings>
        std::vector<Option::SPtr> ref_global_options_bynames(String&& name, Strings&&... names) {
            std::vector<Option::SPtr> out ;

            if(auto opt = ref_global_options_bynames(std::forward<String>(name))) {
                out.push_back(std::move(opt)) ;
            }

            auto append = [&out](auto&& ptr) {
                if(ptr) {
                    out.push_back(std::forward<decltype(ptr)>(ptr)) ;
                }
            } ;

            ((append(ref_global_options_bynames(std::forward<String>(names)))), ...) ;

            return out ;
        }
    }
}

#endif
