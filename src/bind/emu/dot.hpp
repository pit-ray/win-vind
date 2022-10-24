#ifndef _EDI_DOT_HPP
#define _EDI_DOT_HPP

#include "bind/bindedfunc.hpp"


namespace vind
{
    namespace bind
    {
        class RepeatLastChange : public BindedFuncFlex<RepeatLastChange> {
        private:
            static BindedFunc* lastchange_ ;
            static std::uint16_t repeat_count_ ;

        public:
            explicit RepeatLastChange() ;
            static SystemCall sprocess(
                std::uint16_t count, const std::string& args) ;

            inline static void store_change(
                    BindedFunc* const func,
                    std::uint16_t repeat_count=1) noexcept {
                lastchange_   = func ;
                repeat_count_ = repeat_count ;
            }
        } ;
    }
}

#endif
