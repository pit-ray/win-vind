#ifndef _EASY_CLICK_HPP
#define _EASY_CLICK_HPP

#include "bind/base/binded_func_creator.hpp"

#include <windows.h>

#include <memory>

#include "key/keycode_def.hpp"

namespace vind
{
    class EasyClick {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit EasyClick() ;
        virtual ~EasyClick() noexcept ;

        EasyClick(const EasyClick&)            = delete ;
        EasyClick& operator=(const EasyClick&) = delete ;

        EasyClick(EasyClick&&) ;
        EasyClick& operator=(EasyClick&&) ;

        void scan_ui_objects(HWND hwnd) const ;
        void create_matching_loop(KeyCode sendkey=KEYCODE_UNDEFINED) const ;
    } ;


    struct EasyClickLeft : public BindedFuncCreator<EasyClickLeft>, public EasyClick {
        explicit EasyClickLeft() ;
        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
    } ;

    struct EasyClickRight : public BindedFuncCreator<EasyClickRight>, public EasyClick {
        explicit EasyClickRight() ;
        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
    } ;

    struct EasyClickMid : public BindedFuncCreator<EasyClickMid>, public EasyClick {
        explicit EasyClickMid() ;
        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
    } ;

    struct EasyClickHover : public BindedFuncCreator<EasyClickHover>, public EasyClick {
        explicit EasyClickHover() ;
        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;
    } ;
}

#endif
