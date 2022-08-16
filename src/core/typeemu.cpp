#include "typeemu.hpp"

#include "keycode.hpp"

#include "util/keystroke_repeater.hpp"
#include <memory>


namespace vind
{
    namespace core
    {
        struct TypingEmulator::Impl {
            CmdUnit prev_cmdunit_{} ;
            util::KeyStrokeRepeater ksr_{} ;
        } ;

        TypingEmulator::TypingEmulator()
        : pimpl(std::make_unique<Impl>())
        {}

        TypingEmulator::~TypingEmulator() noexcept = default ;

        TypingEmulator::TypingEmulator(TypingEmulator&&) = default ;
        TypingEmulator& TypingEmulator::operator=(TypingEmulator&&) = default ;

        CmdUnit::SPtr TypingEmulator::lowlevel_to_typing(const CmdUnit& raw_cmdunit) {
            if(raw_cmdunit.empty()) {
                pimpl->prev_cmdunit_ = raw_cmdunit ;
                return nullptr ;
            }

            if(raw_cmdunit == pimpl->prev_cmdunit_) {
                if(!pimpl->ksr_.is_passed()) {
                    return nullptr ;
                }
                return std::make_shared<CmdUnit>(raw_cmdunit) ;
            }

            CmdUnitSet system_set{} ;
            CmdUnitSet ascii_set{} ;
            for(const auto& key : raw_cmdunit) {
                if(key.is_major_system()) {
                    system_set.insert(key) ;
                }
                else {
                    ascii_set.insert(key) ;
                }
            }

            auto in_cmdunit = raw_cmdunit - pimpl->prev_cmdunit_ ;
            if(!system_set.empty() && !ascii_set.empty()) {
                // ASCII and System
                auto tmp = in_cmdunit.data() ;
                tmp.merge(system_set) ;
                in_cmdunit = tmp ;
            }

            pimpl->prev_cmdunit_ = raw_cmdunit ;
            pimpl->ksr_.reset() ;

            if(in_cmdunit.empty()) {
                return nullptr ;
            }
            return std::make_shared<CmdUnit>(std::move(in_cmdunit)) ;
        }
    }
}
