#ifndef _CHANGEBASE_HPP
#define _CHANGEBASE_HPP

#include "bind/bindedfunc.hpp"
#include "dot.hpp"


namespace vind
{
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
            SystemCall do_process(
                    std::uint16_t count, const std::string& args) override {
                static_cast<Derived*>(this)->sprocess(count, args) ;
                RepeatLastChange::store_change(this) ;
                return SystemCall::SUCCEEDED ;
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
