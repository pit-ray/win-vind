#ifndef _OPTION_WITH_CREATOR_HPP
#define _OPTION_WITH_CREATOR_HPP

#include "option.hpp"
#include <memory>

namespace vind
{
    //use Curiously Recurring Template Pattern (CRTP)
    //derived class must implement sprocess and sname.
    //if derived class does not use variable of member, sprocess prefers static function.
    //else, sprocess is constant function.
    template <typename Derived>
    class OptionCreator : public Option {
    public:
        static std::unique_ptr<Option> create() {
            return std::make_unique<Derived>() ;
        }

        static std::shared_ptr<Option> create_with_cache() {
            static std::weak_ptr<Derived> cache ;

            auto pobj = cache.lock() ;

            if(!pobj) {
                pobj = std::make_shared<Derived>() ;
                cache = pobj ;
            }

            return pobj ;
        }

        std::string name() const noexcept override {
            return Derived::sname() ;
        }
    } ;
}

#endif
