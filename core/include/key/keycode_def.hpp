#ifndef _KEYCODE_DEF_HPP
#define _KEYCODE_DEF_HPP

namespace vind {
    using KeyCode = unsigned char ;
}

#define KEYCODE_MOUSE_LEFT   (static_cast<vind::KeyCode>(1))
#define KEYCODE_MOUSE_RIGHT  (static_cast<vind::KeyCode>(2))
#define KEYCODE_MOUSE_MID    (static_cast<vind::KeyCode>(4))

#define KEYCODE_0            (static_cast<vind::KeyCode>(48))
#define KEYCODE_1            (static_cast<vind::KeyCode>(49))
#define KEYCODE_2            (static_cast<vind::KeyCode>(50))
#define KEYCODE_3            (static_cast<vind::KeyCode>(51))
#define KEYCODE_4            (static_cast<vind::KeyCode>(52))
#define KEYCODE_5            (static_cast<vind::KeyCode>(53))
#define KEYCODE_6            (static_cast<vind::KeyCode>(54))
#define KEYCODE_7            (static_cast<vind::KeyCode>(55))
#define KEYCODE_8            (static_cast<vind::KeyCode>(56))
#define KEYCODE_9            (static_cast<vind::KeyCode>(57))

#define KEYCODE_MINUS        (static_cast<vind::KeyCode>(189))
#define KEYCODE_HAT          (static_cast<vind::KeyCode>(222))
#define KEYCODE_VBAR         (static_cast<vind::KeyCode>(220))
#define KEYCODE_BKSPACE      (static_cast<vind::KeyCode>(8))

#define KEYCODE_TAB          (static_cast<vind::KeyCode>(9))
#define KEYCODE_Q            (static_cast<vind::KeyCode>(81))
#define KEYCODE_W            (static_cast<vind::KeyCode>(87))
#define KEYCODE_E            (static_cast<vind::KeyCode>(69))
#define KEYCODE_R            (static_cast<vind::KeyCode>(82))
#define KEYCODE_T            (static_cast<vind::KeyCode>(84))
#define KEYCODE_Y            (static_cast<vind::KeyCode>(89))
#define KEYCODE_U            (static_cast<vind::KeyCode>(85))
#define KEYCODE_I            (static_cast<vind::KeyCode>(73))
#define KEYCODE_O            (static_cast<vind::KeyCode>(79))
#define KEYCODE_P            (static_cast<vind::KeyCode>(80))
#define KEYCODE_AT           (static_cast<vind::KeyCode>(192))
#define KEYCODE_LBOX_BRACKET (static_cast<vind::KeyCode>(219))
#define KEYCODE_ENTER        (static_cast<vind::KeyCode>(13))

#define KEYCODE_SHIFT_CAPSLOCK (static_cast<vind::KeyCode>(20))
#define KEYCODE_CAPSLOCK       (static_cast<vind::KeyCode>(240))

#define KEYCODE_A            (static_cast<vind::KeyCode>(65))
#define KEYCODE_S            (static_cast<vind::KeyCode>(83))
#define KEYCODE_D            (static_cast<vind::KeyCode>(68))
#define KEYCODE_F            (static_cast<vind::KeyCode>(70))
#define KEYCODE_G            (static_cast<vind::KeyCode>(71))
#define KEYCODE_H            (static_cast<vind::KeyCode>(72))
#define KEYCODE_J            (static_cast<vind::KeyCode>(74))
#define KEYCODE_K            (static_cast<vind::KeyCode>(75))
#define KEYCODE_L            (static_cast<vind::KeyCode>(76))
#define KEYCODE_PLUS         (static_cast<vind::KeyCode>(187))
#define KEYCODE_COLON        (static_cast<vind::KeyCode>(186))
#define KEYCODE_RBOX_BRACKET (static_cast<vind::KeyCode>(221))

#define KEYCODE_LSHIFT       (static_cast<vind::KeyCode>(160))
#define KEYCODE_Z            (static_cast<vind::KeyCode>(90))
#define KEYCODE_X            (static_cast<vind::KeyCode>(88))
#define KEYCODE_C            (static_cast<vind::KeyCode>(67))
#define KEYCODE_V            (static_cast<vind::KeyCode>(86))
#define KEYCODE_B            (static_cast<vind::KeyCode>(66))
#define KEYCODE_N            (static_cast<vind::KeyCode>(78))
#define KEYCODE_M            (static_cast<vind::KeyCode>(77))

#define KEYCODE_COMMA        (static_cast<vind::KeyCode>(188))
#define KEYCODE_PERIOD       (static_cast<vind::KeyCode>(190))
#define KEYCODE_SLASH        (static_cast<vind::KeyCode>(191))
#define KEYCODE_BKSLASH      (static_cast<vind::KeyCode>(226))
#define KEYCODE_RSHIFT       (static_cast<vind::KeyCode>(161))

#define KEYCODE_LCTRL        (static_cast<vind::KeyCode>(162))
#define KEYCODE_LWIN         (static_cast<vind::KeyCode>(91))
#define KEYCODE_RWIN         (static_cast<vind::KeyCode>(92))
#define KEYCODE_LALT         (static_cast<vind::KeyCode>(164))
#define KEYCODE_NOCONVERT    (static_cast<vind::KeyCode>(29))
#define KEYCODE_SPACE        (static_cast<vind::KeyCode>(32))
#define KEYCODE_CONVERT      (static_cast<vind::KeyCode>(28))
#define KEYCODE_KANA         (static_cast<vind::KeyCode>(242))
#define KEYCODE_KANJI        (static_cast<vind::KeyCode>(25))
#define KEYCODE_RALT         (static_cast<vind::KeyCode>(165))
#define KEYCODE_APP          (static_cast<vind::KeyCode>(93))
#define KEYCODE_RCTRL        (static_cast<vind::KeyCode>(163))

#define KEYCODE_ESC          (static_cast<vind::KeyCode>(27))
#define KEYCODE_F1           (static_cast<vind::KeyCode>(112))
#define KEYCODE_F2           (static_cast<vind::KeyCode>(113))
#define KEYCODE_F3           (static_cast<vind::KeyCode>(114))
#define KEYCODE_F4           (static_cast<vind::KeyCode>(115))
#define KEYCODE_F5           (static_cast<vind::KeyCode>(116))
#define KEYCODE_F6           (static_cast<vind::KeyCode>(117))
#define KEYCODE_F7           (static_cast<vind::KeyCode>(118))
#define KEYCODE_F8           (static_cast<vind::KeyCode>(119))
#define KEYCODE_F9           (static_cast<vind::KeyCode>(120))
#define KEYCODE_F10          (static_cast<vind::KeyCode>(121))
#define KEYCODE_F11          (static_cast<vind::KeyCode>(122))
#define KEYCODE_F12          (static_cast<vind::KeyCode>(123))
#define KEYCODE_F13          (static_cast<vind::KeyCode>(124))
#define KEYCODE_F14          (static_cast<vind::KeyCode>(125))
#define KEYCODE_F15          (static_cast<vind::KeyCode>(126))
#define KEYCODE_F16          (static_cast<vind::KeyCode>(127))
#define KEYCODE_F17          (static_cast<vind::KeyCode>(128))
#define KEYCODE_F18          (static_cast<vind::KeyCode>(129))
#define KEYCODE_F19          (static_cast<vind::KeyCode>(130))
#define KEYCODE_F20          (static_cast<vind::KeyCode>(131))
#define KEYCODE_F21          (static_cast<vind::KeyCode>(132))
#define KEYCODE_F22          (static_cast<vind::KeyCode>(133))
#define KEYCODE_F23          (static_cast<vind::KeyCode>(134))
#define KEYCODE_F24          (static_cast<vind::KeyCode>(135))

#define KEYCODE_SNAPSHOT     (static_cast<vind::KeyCode>(44))
#define KEYCODE_SCROLL       (static_cast<vind::KeyCode>(145))
#define KEYCODE_PAUSE        (static_cast<vind::KeyCode>(19))
#define KEYCODE_INSERT       (static_cast<vind::KeyCode>(45))
#define KEYCODE_HOME         (static_cast<vind::KeyCode>(36))
#define KEYCODE_PAGEUP       (static_cast<vind::KeyCode>(33))
#define KEYCODE_DELETE       (static_cast<vind::KeyCode>(46))
#define KEYCODE_END          (static_cast<vind::KeyCode>(35))
#define KEYCODE_PAGEDOWN     (static_cast<vind::KeyCode>(34))

#define KEYCODE_LEFT         (static_cast<vind::KeyCode>(37))
#define KEYCODE_UP           (static_cast<vind::KeyCode>(38))
#define KEYCODE_RIGHT        (static_cast<vind::KeyCode>(39))
#define KEYCODE_DOWN         (static_cast<vind::KeyCode>(40))

#define KEYCODE_NUMLOCK      (static_cast<vind::KeyCode>(144))

#define KEYCODE_FROM_JP      (static_cast<vind::KeyCode>(244))
#define KEYCODE_TO_EN        (static_cast<vind::KeyCode>(243))
#define KEYCODE_FROM_EN      (static_cast<vind::KeyCode>(243))
#define KEYCODE_TO_JP        (static_cast<vind::KeyCode>(244))
#define KEYCODE_IME          (static_cast<vind::KeyCode>(14))

#define KEYCODE_BACKTAB      (static_cast<vind::KeyCode>(245))

//
// Keyboards with two Windows-keys are rare, so we don't support them.
// Replated Code: @Double-Win-Key
//
//#define KEYCODE_WIN          (static_cast<vind::KeyCode>(15))

#define KEYCODE_SHIFT        (static_cast<vind::KeyCode>(16))
#define KEYCODE_CTRL         (static_cast<vind::KeyCode>(17))
#define KEYCODE_ALT          (static_cast<vind::KeyCode>(18))
#define KEYCODE_OPTIONAL     (static_cast<vind::KeyCode>(177))
#define KEYCODE_OPTNUMBER    (static_cast<vind::KeyCode>(178))

#define KEYCODE_UNDEFINED    (static_cast<vind::KeyCode>(0))

#endif
