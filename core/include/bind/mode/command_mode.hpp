#ifndef _COMMAND_MODE_HPP
#define _COMMAND_MODE_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    class CommandMode : public BindedFuncCreator<CommandMode> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit CommandMode() ;
        static const std::string sname() noexcept ;

        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        virtual ~CommandMode() noexcept ;
        CommandMode(CommandMode&&) ;
        CommandMode& operator=(CommandMode&&) ;
        CommandMode(const CommandMode&)            = delete ;
        CommandMode& operator=(const CommandMode&) = delete ;
    } ;
}

#endif
