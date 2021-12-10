#ifndef _EDI_DOT_HPP
#define _EDI_DOT_HPP

#include "bind/binded_func.hpp"


namespace vind
{
    namespace bind
    {
        class RepeatLastChange : public BindedFuncFlex<RepeatLastChange> {
        private:
            static const BindedFunc* lastchange_ ;
            static unsigned int repeat_count_ ;

        public:
            explicit RepeatLastChange() ;
            static core::SystemCall sprocess() ;
            static core::SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static core::SystemCall sprocess(const core::CharLogger& parent_lgr) ;

            inline static void store_change(
                    const BindedFunc* const func,
                    unsigned int repeat_count=1) noexcept {
                lastchange_   = func ;
                repeat_count_ = repeat_count ;
            }
        } ;
    }
}

#endif
