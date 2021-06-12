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
        explicit KeyStrokeRepeater(int wait_time_for_starting_ms=512) ;
        virtual ~KeyStrokeRepeater() noexcept ;

        KeyStrokeRepeater(KeyStrokeRepeater&&) ;
        KeyStrokeRepeater& operator=(KeyStrokeRepeater&&) ;

        KeyStrokeRepeater(const KeyStrokeRepeater&) ;
        KeyStrokeRepeater& operator=(const KeyStrokeRepeater&) ;

        void set_wait_time(int delta_ms=512) ;
        int get_wait_time_ms() const noexcept ;

        virtual void reset() noexcept ;
        virtual bool is_passed() const ;
    } ;
}

#endif
