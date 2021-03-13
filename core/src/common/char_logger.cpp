#include "char_logger.hpp"

#include "key_absorber.hpp"
#include "key_logger_base.hpp"
#include "keystroke_repeater.hpp"
#include "vkc_converter.hpp"

enum FlagBits
{
    CHAR_CHANGED     = 0x01,
    KEYSTROKE_CHANGED = 0x10,
} ;

struct CharLogger::Impl
{
    KeyLog prelog{} ;
    unsigned char flags = FlagBits::KEYSTROKE_CHANGED ;
    KeyStrokeRepeater ksr{} ;
} ;

CharLogger::CharLogger()
: KeyLoggerBase(),
  pimpl(std::make_unique<Impl>())
{}

CharLogger::~CharLogger() noexcept = default ;

CharLogger::CharLogger(const CharLogger& rhs)
: KeyLoggerBase(rhs),
  pimpl(rhs.pimpl ? std::make_unique<Impl>(*(rhs.pimpl)) : std::make_unique<Impl>())
{}

CharLogger& CharLogger::operator=(const CharLogger& rhs)
{
    if(rhs.pimpl) {
        KeyLoggerBase::operator=(rhs) ;
        *pimpl = *(rhs.pimpl) ;
    }
    return *this ;
}

CharLogger::CharLogger(CharLogger&&)            = default ;
CharLogger& CharLogger::operator=(CharLogger&&) = default ;

void CharLogger::update()
{
    static const KeyLog cl_toggles(VKCConverter::get_toggle_keys()) ;

    auto log = KeyAbsorber::get_pressed_list() - cl_toggles ;

    if(log != pimpl->prelog) { //type is changed
        pimpl->flags |= FlagBits::KEYSTROKE_CHANGED ; //true

        const auto diff = log - pimpl->prelog ;
        pimpl->prelog = log ;

        if(!log.is_containing(VKC_SHIFT)) {
            logging(std::move(diff)) ;
            pimpl->flags |= FlagBits::CHAR_CHANGED ; //true
            return ;
        }

        //shfited
        auto data = diff.get() ;
        data.insert(VKC_SHIFT) ;

        //construct KeyLog inside logs directly from std::vector
        logging(std::move(data)) ;
        pimpl->flags |= CHAR_CHANGED ; //true
    }
    else { //long pressing
        logging(log) ;
        if(pimpl->flags & FlagBits::KEYSTROKE_CHANGED) {
            pimpl->flags &= !(FlagBits::CHAR_CHANGED | FlagBits::KEYSTROKE_CHANGED) ; //make all false
            pimpl->ksr.reset() ;
            return ;
        }

        //emulate key stroke
        if(pimpl->ksr.is_pressed())
            pimpl->flags |= FlagBits::CHAR_CHANGED ;
        else
            pimpl->flags &= !FlagBits::CHAR_CHANGED ;
    }
}

bool CharLogger::is_changed() const noexcept
{
    return pimpl->flags & FlagBits::CHAR_CHANGED ;
}

const std::string CharLogger::to_str() const
{
    if(empty()) return "" ;

    std::string str{} ;
    for(auto itr = cbegin() ; itr != cend() ; itr ++) {
        if(itr->is_containing(VKC_SHIFT)) {
            //shifted ascii
            for(const auto vkc : *itr) {
                const auto c = VKCConverter::get_shifted_ascii(vkc) ;
                if(c != 0) str.push_back(c) ;
            }
            continue ;
        }
        for(const auto vkc : *itr) {
            const auto c = VKCConverter::get_ascii(vkc) ;
            if(c != 0) str.push_back(c) ;
        }
    }
    return str ;
}
