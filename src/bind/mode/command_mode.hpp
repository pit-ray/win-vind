#ifndef _COMMAND_MODE_HPP
#define _COMMAND_MODE_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    class ToCommand : public BindedFuncCreator<ToCommand> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit ToCommand() ;

        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        virtual ~ToCommand() noexcept ;
        ToCommand(ToCommand&&) ;
        ToCommand& operator=(ToCommand&&) ;
        ToCommand(const ToCommand&)            = delete ;
        ToCommand& operator=(const ToCommand&) = delete ;

        void reconstruct() override ;
    } ;
}

#endif
