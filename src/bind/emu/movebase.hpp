#ifndef _MOVEBASE_HPP
#define _MOVEBASE_HPP

#include "bind/bindedfunc.hpp"


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
            SystemCall do_process(
                    std::uint16_t count, const std::string& args) override {
                static_cast<Derived*>(this)->sprocess(count, args) ;
                return SystemCall::NOTHING ;
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
