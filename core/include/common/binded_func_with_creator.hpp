#ifndef _BINDED_FUNC_WITH_CREATOR_HPP
#define _BINDED_FUNC_WITH_CREATOR_HPP

#include <memory>
#include "binded_func.hpp"

//use Curiously Recurring Template Pattern (CRTP)
//derived class must implement sprocess and sname.
//if derived class does not use variable of member, sprocess prefers static function.
//else, sprocess is constant function.
template <typename Derived>
class BindedFuncWithCreator : public BindedFunc {
private:
    void do_process(
            const bool first_call,
            const unsigned int repeat_num,
            KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const override {
        static_cast<const Derived*>(this)->sprocess(
                first_call, repeat_num, parent_vkclgr, parent_charlgr) ;
    }

public:
    static std::unique_ptr<BindedFunc> create() {
        return std::make_unique<Derived>() ;
    }

    static std::shared_ptr<BindedFunc> create_with_cache() {
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
