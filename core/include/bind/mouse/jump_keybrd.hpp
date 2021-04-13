#ifndef _JUMP_KEYBRD_HPP
#define _JUMP_KEYBRD_HPP

#include "bind/base/binded_func_with_creator.hpp"

namespace vind
{
    class Jump2Any : public BindedFuncCreator<Jump2Any> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit Jump2Any() ;
        virtual ~Jump2Any() noexcept ;

        Jump2Any(Jump2Any&&) ;
        Jump2Any& operator=(Jump2Any&&) ;

        Jump2Any(const Jump2Any&)            = delete ;
        Jump2Any& operator=(const Jump2Any&) = delete ;

        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
        static const std::string sname() noexcept ;

        void load_config() override ;
    } ;
}

#endif
