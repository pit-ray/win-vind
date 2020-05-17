#include "edi_move_cursor.hpp"
#include "keybrd_eventer.hpp"
#include "interval_timer.hpp"
#include "msg_logger.hpp"
#include "default_config.hpp"

using namespace std ;


namespace EdiMoveUtility
{
    static constexpr auto DELTA_US = 130000 ;
    inline static void disp_error_msg() noexcept {
        Logger::error_stream << "[Error] windows.h: " \
        << GetLastError() \
        << " (bf_edi_move_cursor.cpp)\n" ;
    }

    inline static POINT get_pos_num() noexcept {
        POINT pos{0, 0} ;
        if(!GetCursorPos(&pos)) {
            disp_error_msg() ;
            return POINT{0, 0} ;
        }
        const auto hwnd = WindowFromPoint(pos) ;

        TCHAR inline_num{0} ;
        if(!SendMessage(hwnd, EM_LINELENGTH, inline_num, 0)) {
            disp_error_msg() ;
            return POINT{0, 0} ;
        }
        cout << static_cast<int>(inline_num) << endl ;

        DWORD32 current_pos{0} ;
        if(SendMessage(hwnd, EM_GETSEL, 0, reinterpret_cast<LPARAM>(&current_pos)) < 0) {
            disp_error_msg() ;
            return POINT{0, 0} ;
        }
        cout << current_pos << endl ;

        return POINT{current_pos, inline_num} ;
    }
}

using namespace EdiMoveUtility ;

//EdiMoveLeft
struct EdiMoveLeft::Impl
{
    IntervalTimer timer{30} ;
} ;

EdiMoveLeft::EdiMoveLeft()
: pimpl(make_unique<Impl>())
{}

EdiMoveLeft::~EdiMoveLeft() = default ;

EdiMoveLeft::EdiMoveLeft(EdiMoveLeft&&) = default ;
EdiMoveLeft& EdiMoveLeft::operator=(EdiMoveLeft&&) = default ;

const string EdiMoveLeft::sname() noexcept
{
    return "edi_move_left" ;
}

bool EdiMoveLeft::sprocess(const bool first_call) const
{
    if(first_call) {
        pimpl->timer.reset() ;
    }

    if(!pimpl->timer.is_passed()) return true ;

    const auto posnum = get_pos_num() ;
    if((posnum.x + 1) > posnum.y) {
        return true ;
    }

    if(!KeybrdEventer::is_pushup(VK_LEFT)) {
        return false ;
    }
    return true ;
}


//EdiMoveRight
struct EdiMoveRight::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

EdiMoveRight::EdiMoveRight()
: pimpl(make_unique<Impl>())
{}

EdiMoveRight::~EdiMoveRight() = default ;

EdiMoveRight::EdiMoveRight(EdiMoveRight&&) = default ;
EdiMoveRight& EdiMoveRight::operator=(EdiMoveRight&&) = default ;

const string EdiMoveRight::sname() noexcept
{
    return "edi_move_right" ;
}

bool EdiMoveRight::sprocess(const bool first_call) const
{
    /*
    if(first_call) {
        pimpl->timer.reset() ;
    }

    if(!pimpl->timer.is_passed()) return true ;

    const auto posnum = get_pos_num() ;
    if(posnum.x == 0) {
        return true ;
    }

    if(!KeybrdEventer::is_pushup(VK_RIGHT)) {
        return false ;
    }

    return true ;
    */
    return true ;
}



//EdiMoveUp
struct EdiMoveUp::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

EdiMoveUp::EdiMoveUp()
: pimpl(make_unique<Impl>())
{}

EdiMoveUp::~EdiMoveUp() = default ;

EdiMoveUp::EdiMoveUp(EdiMoveUp&&) = default ;
EdiMoveUp& EdiMoveUp::operator=(EdiMoveUp&&) = default ;

const string EdiMoveUp::sname() noexcept
{
    return "edi_move_up" ;
}

bool EdiMoveUp::sprocess(const bool first_call) const
{
    /*
    if(first_call) {
        pimpl->timer.reset() ;
    }

    if(!pimpl->timer.is_passed()) return true ;

    POINT pos{0, 0} ;
    if(!GetCursorPos(&pos)) {
        disp_error_msg() ;
        return false ;
    }
    const auto hwnd = WindowFromPoint(pos) ;
    if(!CreateCaret(hwnd, NULL, 0, 0)) {
        disp_error_msg() ;
        return false ;
    }

    POINT caret_pos{0, 0} ;
    if(!GetCaretPos(&caret_pos)) {
        disp_error_msg() ;
        return false ;
    }

    ShowCaret(hwnd) ;

    return true ;
    */
    return true ;
}



//EdiMoveDown
struct EdiMoveDown::Impl
{
    IntervalTimer timer{DELTA_US} ;
} ;

EdiMoveDown::EdiMoveDown()
: pimpl(make_unique<Impl>())
{}

EdiMoveDown::~EdiMoveDown() = default ;

EdiMoveDown::EdiMoveDown(EdiMoveDown&&) = default ;
EdiMoveDown& EdiMoveDown::operator=(EdiMoveDown&&) = default ;

const string EdiMoveDown::sname() noexcept
{
    return "edi_move_down" ;
}

bool EdiMoveDown::sprocess(const bool first_call) const
{
    /*
    if(first_call) {
        pimpl->timer.reset() ;
    }

    if(!pimpl->timer.is_passed()) return true ;

    const auto posnum = get_pos_num() ;

    if(!KeybrdEventer::is_pushup(VK_DOWN)) {
        return false ;
    }

    for(unsigned int i = 0 ; i < posnum.x ; i ++) {
        if(!KeybrdEventer::is_pushup(VK_RIGHT)) {
            return false ;
        }
        if((i + 1) > posnum.y) {
            break ;
        }
    }
    */
    return true ;
}