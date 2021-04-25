#ifndef _KEYSTROKE_REPEATER_HPP
#define _KEYSTROKE_REPEATER_HPP

#include <memory>

namespace vind
{
    class KeyStrokeRepeater {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit KeyStrokeRepeater(unsigned int wait_time_for_starting_ms=512) ;
        virtual ~KeyStrokeRepeater() noexcept ;

        KeyStrokeRepeater(KeyStrokeRepeater&&) ;
        KeyStrokeRepeater& operator=(KeyStrokeRepeater&&) ;

        KeyStrokeRepeater(const KeyStrokeRepeater&) ;
        KeyStrokeRepeater& operator=(const KeyStrokeRepeater&) ;

        virtual void reset() noexcept ;
        virtual bool is_pressed() const ;
    } ;
}

#endif
