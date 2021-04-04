#ifndef _JUMP_KEYBRD_HPP
#define _JUMP_KEYBRD_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    class Jump2Any : public BindedFuncWithCreator<Jump2Any> {
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

        void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_keycodelgr,
                const CharLogger* const parent_charlgr) const ;
        static const std::string sname() noexcept ;

        void load_config() override ;
    } ;
}

#endif
