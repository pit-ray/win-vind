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

            /**
             * It converts the raw keycodes duplicated sequentially
             * to the command unit as if typing.
             */
            CmdUnit::SPtr lowlevel_to_typing(
                const CmdUnit& raw_cmdunit) ;
        } ;
    }
}

#endif
