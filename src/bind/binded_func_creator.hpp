#ifndef _BINDED_FUNC_WITH_CREATOR_HPP
#define _BINDED_FUNC_WITH_CREATOR_HPP

#include "binded_func.hpp"
#include "util/string.hpp"


namespace vind
{
    class NTypeLogger ;
    class CharLogger ;

    //use Curiously Recurring Template Pattern (CRTP)
    //derived class must implement sprocess() and sname().
    //If derived class does not have any variables of member, sprocess() prefers static function.
    //If not, sprocess() is constant function.
    template <typename Derived>
    class BindedFuncCreator : public BindedFunc {
    private:
        void do_process() const override {
            static_cast<const Derived*>(this)->sprocess() ;
        }
        void do_process(NTypeLogger& parent_lgr) const override {
            static_cast<const Derived*>(this)->sprocess(parent_lgr) ;
        }
        void do_process(const CharLogger& parent_lgr) const override {
            static_cast<const Derived*>(this)->sprocess(parent_lgr) ;
        }

    public:
        explicit BindedFuncCreator(const std::string& name)
        : BindedFunc(util::A2a(name))
        {}
        explicit BindedFuncCreator(std::string&& name)
        : BindedFunc(util::A2a(std::move(name)))
        {}

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
    } ;
}

#endif
