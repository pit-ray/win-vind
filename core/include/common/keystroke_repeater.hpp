#ifndef _KEYSTROKE_REPEATER_HPP
#define _KEYSTROKE_REPEATER_HPP

#include <memory>

class KeyStrokeRepeater
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    explicit KeyStrokeRepeater(const unsigned int wait_time_for_starting_ms=512) ;
    virtual ~KeyStrokeRepeater() noexcept ;

    KeyStrokeRepeater(KeyStrokeRepeater&&) ;
    KeyStrokeRepeater& operator=(KeyStrokeRepeater&&) ;

    KeyStrokeRepeater(const KeyStrokeRepeater&)            = delete ;
    KeyStrokeRepeater& operator=(const KeyStrokeRepeater&) = delete ;

    void reset() noexcept ;
    bool is_pressed() ;
} ;


#endif
