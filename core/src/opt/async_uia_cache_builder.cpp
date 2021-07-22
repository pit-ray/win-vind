#include "opt/async_uia_cache_builder.hpp"

#if defined(DEBUG)
#include <iostream>
#endif


namespace
{

}


namespace vind
{
    AsyncUIACacheBuilder::AsyncUIACacheBuilder()
    : OptionCreator("uia_cachebuild")
    {}

    void AsyncUIACacheBuilder::do_enable() const {
        std::cout << "enabled\n" ;

        get_root_element(NULL) ;

        register_properties(10, 20, 30) ;
    }

    void AsyncUIACacheBuilder::do_disable() const {

    }

    void AsyncUIACacheBuilder::do_process() const {
    }

    void AsyncUIACacheBuilder::register_property(PROPERTYID id) {
        std::cout << id << std::endl ;
    }

    SmartElement AsyncUIACacheBuilder::get_root_element(HWND hwnd) {
        std::cout << "called\n" ;
        return SmartElement(nullptr) ;
    }
}
