#ifndef _TYPEEMU_HPP
#define _TYPEEMU_HPP

#include "cmdunit.hpp"
#include <memory>

namespace vind
{
    namespace core
    {
        class TypingEmulator {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit TypingEmulator() ;
            virtual ~TypingEmulator() noexcept ;

            TypingEmulator(const TypingEmulator&) = delete ;
            TypingEmulator& operator=(const TypingEmulator&) = delete ;

            TypingEmulator(TypingEmulator&&) ;
            TypingEmulator& operator=(TypingEmulator&&) ;

            CmdUnit::SPtr lowlevel_to_typing(
                const CmdUnit& raw_cmdunit) ;

            bool ignore_if_syskey_only(const CmdUnit& in_cmdunit) ;
        } ;
    }
}

#endif
