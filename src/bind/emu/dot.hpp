#ifndef _EDI_DOT_HPP
#define _EDI_DOT_HPP

#include "bind/bindedfunc.hpp"


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
            static SystemCall sprocess() ;
            static SystemCall sprocess(core::NTypeLogger& parent_lgr) ;
            static SystemCall sprocess(const core::CharLogger& parent_lgr) ;

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
