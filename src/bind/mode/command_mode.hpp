#ifndef _COMMAND_MODE_HPP
#define _COMMAND_MODE_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace bind
    {
        class ToCommand : public BindedFuncCreator<ToCommand> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit ToCommand() ;

            void sprocess() const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            virtual ~ToCommand() noexcept ;
            ToCommand(ToCommand&&) ;
            ToCommand& operator=(ToCommand&&) ;
            ToCommand(const ToCommand&)            = delete ;
            ToCommand& operator=(const ToCommand&) = delete ;

            void reconstruct() override ;
        } ;
    }
}

#endif
