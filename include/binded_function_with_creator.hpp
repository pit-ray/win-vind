#ifndef _BINDED_FUNCTION_WITH_CREATOR_HPP
#define _BINDED_FUNCTION_WITH_CREATOR_HPP

#include "binded_function.hpp"

#include <memory>

//use Curiously Recurring Template Pattern (CRTP)
//derived class must implement sprocess and sname.
//if derived class does not use variable of member, sprocess prefers static function.
//else, sprocess is constant function.
template <typename Derived>
class BindedFunctionWithCreator : public BindedFunction {
private:
    bool do_process(const bool first_call) const override {
        return static_cast<const Derived*>(this)->sprocess(first_call) ;
    }

public:
    static std::unique_ptr<BindedFunction> create() {
        return std::move(std::make_unique<Derived>()) ;
    }

    static std::shared_ptr<BindedFunction> create_with_cache() {
        static std::weak_ptr<Derived> cache ;

        auto pobj = cache.lock() ;

        if(!pobj) {
            pobj = std::make_shared<Derived>() ;
            cache = pobj ;
        }

        return pobj ;
    }

    const std::string name() const noexcept override {
        return Derived::sname() ;
    }
} ;


#endif