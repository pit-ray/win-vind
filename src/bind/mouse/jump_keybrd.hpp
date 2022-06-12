#ifndef _JUMP_KEYBRD_HPP
#define _JUMP_KEYBRD_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        class JumpWithKeybrdLayout : public BindedFuncVoid<JumpWithKeybrdLayout> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit JumpWithKeybrdLayout() ;
            virtual ~JumpWithKeybrdLayout() noexcept ;

            JumpWithKeybrdLayout(JumpWithKeybrdLayout&&) ;
            JumpWithKeybrdLayout& operator=(JumpWithKeybrdLayout&&) ;

            JumpWithKeybrdLayout(const JumpWithKeybrdLayout&)            = delete ;
            JumpWithKeybrdLayout& operator=(const JumpWithKeybrdLayout&) = delete ;

            void sprocess() ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            void reconstruct() override ;
        } ;
    }
}

#endif
