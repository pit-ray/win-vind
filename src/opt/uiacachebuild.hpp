#ifndef _ASYNC_UIA_CACHE_BUILDER_HPP
#define _ASYNC_UIA_CACHE_BUILDER_HPP

#include "option.hpp"
#include "uicache.hpp"
#include "util/uia.hpp"

#include <initializer_list>
#include <unordered_map>
#include <unordered_set>


namespace vind
{
    namespace opt
    {
        class AsyncUIACacheBuilder : public OptionCreator<AsyncUIACacheBuilder> {
        private:
            void do_enable() const override ;
            void do_disable() const override ;
            void do_process() const override ;

            static std::unordered_map<HWND, UICache> caches_ ;

            static inline auto has_cache(HWND hwnd) noexcept {
                return caches_.find(hwnd) != caches_.end() ;
            }

        public:
            explicit AsyncUIACacheBuilder() ;

            static void register_property(PROPERTYID id) ;

            template <typename T>
            static inline auto register_properties(T&& ids) {
                for(auto& id : ids) {
                    register_property(static_cast<PROPERTYID>(id)) ;
                }
            }

            template <typename T>
            static inline auto register_properties(std::initializer_list<T>&& ids) {
                for(auto& id : ids) {
                    register_property(static_cast<PROPERTYID>(id)) ;
                }
            }

            template <typename ...Args>
            static inline auto register_properties(Args&&... ids) {
                return register_properties({ids...}) ;
            }

            static util::SmartElement get_root_element(HWND hwnd) ;
        } ;
    }
}

#endif
