#ifndef _BINDED_FUNC_WITH_CREATOR_HPP
#define _BINDED_FUNC_WITH_CREATOR_HPP

#include "binded_func.hpp"


namespace vind
{
    namespace core
    {
        class NTypeLogger ;
        class CharLogger ;
    }

    namespace bind
    {
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
            void do_process(core::NTypeLogger& parent_lgr) const override {
                static_cast<const Derived*>(this)->sprocess(parent_lgr) ;
            }
            void do_process(const core::CharLogger& parent_lgr) const override {
                static_cast<const Derived*>(this)->sprocess(parent_lgr) ;
            }

        public:
            template <typename String>
            explicit BindedFuncCreator(String&& name)
            : BindedFunc(std::forward<String>(name))
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
}

#endif
