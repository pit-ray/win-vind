#ifndef _ASYNC_UIA_CACHE_BUILDER_HPP
#define _ASYNC_UIA_CACHE_BUILDER_HPP

#include "option_creator.hpp"
#include "uia/uia.hpp"

#include <initializer_list>


namespace vind
{
    class AsyncUIACacheBuilder : public OptionCreator<AsyncUIACacheBuilder>
    {
    private:
        void do_enable() const override ;
        void do_disable() const override ;
        void do_process() const override ;

    public:
        explicit AsyncUIACacheBuilder() ;

        static void register_property(PROPERTYID id) ;

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

        static SmartElement get_root_element(HWND hwnd) ;
    } ;
}

#endif
