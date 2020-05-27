#include "text_selecter.hpp"

#include <mutex>

#include "keybrd_eventer.hpp"

namespace TextSelecter
{
    static KeybrdEventer::SmartKey _selecter{VKC_LSHIFT} ;
    static Mode _mode{Mode::UNSELECT} ;
    static std::mutex _mtx{} ;

    using lock_t = std::lock_guard<std::mutex> ;

    //[negative] up from the line starting of selection.
    //[positive] down from ...
    static int _line_index = 0 ;

    bool is_select_words() noexcept {
        lock_t lock(_mtx) ;

        _line_index = 0 ;
        if(!_selecter.is_release()){
            return false ;
        }

        _mode = Mode::WORD ;
        return _selecter.is_push() ;;
    }

    bool is_select_line_EOL2BOL() noexcept {
        lock_t lock(_mtx) ;

        _line_index = 0 ;
        if(!_selecter.is_release()){
            return false ;
        }

        using namespace KeybrdEventer ;

        if(!is_pushup(VKC_END)) {
            return false ;
        }

        if(!_selecter.is_push()) {
            return false ;
        }

        if(!is_pushup(VKC_HOME)) {
            return false ;
        }

        _mode = Mode::EOL2BOL ;
        return true ;
    }

    bool is_select_line_BOL2EOL() noexcept {
        lock_t lock(_mtx) ;

        _line_index = 0 ;
        if(!_selecter.is_release()){
            return false ;
        }

        using namespace KeybrdEventer ;

        if(!is_pushup(VKC_HOME)) {
            return false ;
        }

        if(!_selecter.is_push()) {
            return false ;
        }

        if(!is_pushup(VKC_END)) {
            return false ;
        }

        _mode = Mode::BOL2EOL ;
        return true ;
    }

    bool is_unselect() noexcept {
        lock_t lock(_mtx) ;

        _mode = Mode::UNSELECT ;
        _line_index = 0 ;
        return _selecter.is_release() ;
    }

    Mode get_mode() noexcept {
        lock_t lock(_mtx) ;

        return _mode ;
    }
}