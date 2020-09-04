#ifndef _KEYSTROKE_REPEATER_HPP
#define _KEYSTROKE_REPEATER_HPP

#include <memory>

class KeyStrokeRepeater
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

public:
    explicit KeyStrokeRepeater() ;
    ~KeyStrokeRepeater() noexcept ;

    KeyStrokeRepeater(KeyStrokeRepeater&&) noexcept ;
    KeyStrokeRepeater& operator=(KeyStrokeRepeater&&) noexcept ;

    KeyStrokeRepeater(const KeyStrokeRepeater&)            = delete ;
    KeyStrokeRepeater& operator=(const KeyStrokeRepeater&) = delete ;

    void reset() noexcept ;

    bool is_pressed() noexcept ;
} ;


#endif