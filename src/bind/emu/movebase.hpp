#ifndef _MOVEBASE_HPP
#define _MOVEBASE_HPP

#include "bind/binded_func.hpp"


namespace vind
{
    namespace core
    {
        class NTypeLogger ;
        class CharLogger ;
    }

    namespace bind
    {
        struct MoveBase : public BindedFunc {
            template <typename String>
            explicit MoveBase(String&& name)
            : BindedFunc(std::forward<String>(name))
            {}
        } ;

        template <typename Derived>
        class MoveBaseCreator : public MoveBase {
        private:
            core::SystemCall do_process() const override {
                static_cast<const Derived*>(this)->sprocess() ;
                return core::SystemCall::NOTHING ;
            }
            core::SystemCall do_process(core::NTypeLogger& parent_lgr) const override {
                static_cast<const Derived*>(this)->sprocess(parent_lgr) ;
                return core::SystemCall::NOTHING ;
            }
            core::SystemCall do_process(const core::CharLogger& parent_lgr) const override {
                static_cast<const Derived*>(this)->sprocess(parent_lgr) ;
                return core::SystemCall::NOTHING ;
            }

        public:
            template <typename String>
            explicit MoveBaseCreator(String&& name)
            : MoveBase(std::forward<String>(name))
            {}

            static std::unique_ptr<BindedFunc> create() {
                return std::make_unique<Derived>() ;
            }
        } ;
    } // namespace bind
} // namespace vind


#endif
