#include "typeemu.hpp"

#include "keycode.hpp"

#include "util/keystroke_repeater.hpp"


namespace vind
{
    namespace core
    {
        struct TypingEmulator::Impl {
            CmdUnit l1_cmdunit_{} ;
            CmdUnit l2_cmdunit_{} ;
            util::KeyStrokeRepeater ksr_{} ;
        } ;

        TypingEmulator::TypingEmulator()
        : pimpl(std::make_unique<Impl>())
        {}

        TypingEmulator::~TypingEmulator() noexcept = default ;

        TypingEmulator::TypingEmulator(TypingEmulator&&) = default ;
        TypingEmulator& TypingEmulator::operator=(TypingEmulator&&) = default ;

        CmdUnit::SPtr TypingEmulator::lowlevel_to_typing(const CmdUnit& raw_cmdunit) {
            auto in_cmdunit = raw_cmdunit - pimpl->l1_cmdunit_ ;
            pimpl->l2_cmdunit_ = pimpl->l1_cmdunit_ ;
            pimpl->l1_cmdunit_ = raw_cmdunit ;

            if(raw_cmdunit.empty()) {
                pimpl->ksr_.reset() ;
                return nullptr ;
            }

            if(in_cmdunit.empty()) {
                if(!pimpl->ksr_.is_passed()) {
                    return nullptr ;
                }
                in_cmdunit = raw_cmdunit ;
            }

            return std::make_shared<CmdUnit>(std::move(in_cmdunit)) ;
        }

        bool TypingEmulator::ignore_if_syskey_only(const CmdUnit& in_cmdunit) {
            bool only_syskey = true ;
            for(const auto& key : in_cmdunit) {
                if(!key.is_major_system()) {
                    only_syskey = false ;
                    break ;
                }
            }

            if(only_syskey) {
                pimpl->l1_cmdunit_ = pimpl->l2_cmdunit_ ;
            }
            return only_syskey ;
        }
    }
}
