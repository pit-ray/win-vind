#ifndef _BINDED_FUNC_WITH_CREATOR_HPP
#define _BINDED_FUNC_WITH_CREATOR_HPP

#include "binded_func.hpp"

namespace vind
{
    //use Curiously Recurring Template Pattern (CRTP)
    //derived class must implement sprocess() and sname().
    //If derived class does not have any variables of member, sprocess() prefers static function.
    //If not, sprocess() is constant function.
    template <typename Derived>
    class BindedFuncWithCreator : public BindedFunc {
    private:
        void do_process(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* parent_keycodelgr,
                const CharLogger* const parent_charlgr) const override {
            static_cast<const Derived*>(this)->sprocess(
                    first_call, repeat_num, parent_keycodelgr, parent_charlgr) ;
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
}

#endif
