#ifndef _CHANGEBASE_HPP
#define _CHANGEBASE_HPP

#include "bind/bindedfunc.hpp"
#include "dot.hpp"


namespace vind
{
    namespace core
    {
        class NTypeLogger ;
        class CharLogger ;
    }

    namespace bind
    {
        struct ChangeBase : public BindedFunc {
            template <typename String>
            explicit ChangeBase(String&& name)
            : BindedFunc(std::forward<String>(name))
            {}
        } ;

        template <typename Derived>
        class ChangeBaseCreator : public ChangeBase {
        private:
            SystemCall do_process() override {
                static_cast<Derived*>(this)->sprocess() ;
                return SystemCall::NOTHING ;
            }
            SystemCall do_process(core::NTypeLogger& parent_lgr) override {
                static_cast<Derived*>(this)->sprocess(parent_lgr) ;
                RepeatLastChange::store_change(this) ;
                return SystemCall::NOTHING ;
            }
            SystemCall do_process(const core::CharLogger& parent_lgr) override {
                static_cast<Derived*>(this)->sprocess(parent_lgr) ;
                RepeatLastChange::store_change(this) ;
                return SystemCall::NOTHING ;
            }

        public:
            template <typename String>
            explicit ChangeBaseCreator(String&& name)
            : ChangeBase(std::forward<String>(name))
            {}

            static std::unique_ptr<BindedFunc> create() {
                return std::make_unique<Derived>() ;
            }
        } ;
    } // namespace bind
} // namespace vind

#endif
