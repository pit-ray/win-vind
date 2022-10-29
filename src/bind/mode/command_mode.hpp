#ifndef _COMMAND_MODE_HPP
#define _COMMAND_MODE_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        class ToCommand : public BindedFuncFlex<ToCommand> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit ToCommand() ;
            SystemCall sprocess(
                std::uint16_t count, const std::string& args) ;

            virtual ~ToCommand() noexcept ;
            ToCommand(ToCommand&&) ;
            ToCommand& operator=(ToCommand&&) ;
            ToCommand(const ToCommand&)            = delete ;
            ToCommand& operator=(const ToCommand&) = delete ;
        } ;
    }
}

#endif
