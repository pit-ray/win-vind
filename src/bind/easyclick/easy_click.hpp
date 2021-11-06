#ifndef _EASY_CLICK_HPP
#define _EASY_CLICK_HPP

#include "bind/binded_func_creator.hpp"

#include <windows.h>

#include <memory>

#include "core/keycode_def.hpp"

namespace vind
{

    class EasyClickLeft : public BindedFuncCreator<EasyClickLeft> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit EasyClickLeft() ;
        virtual ~EasyClickLeft() noexcept ;

        EasyClickLeft(const EasyClickLeft&)            = delete ;
        EasyClickLeft& operator=(const EasyClickLeft&) = delete ;

        EasyClickLeft(EasyClickLeft&&) ;
        EasyClickLeft& operator=(EasyClickLeft&&) ;

        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        void reconstruct() override ;
    } ;

    class EasyClickRight : public BindedFuncCreator<EasyClickRight> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit EasyClickRight() ;
        virtual ~EasyClickRight() noexcept ;

        EasyClickRight(const EasyClickRight&)            = delete ;
        EasyClickRight& operator=(const EasyClickRight&) = delete ;

        EasyClickRight(EasyClickRight&&) ;
        EasyClickRight& operator=(EasyClickRight&&) ;

        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        void reconstruct() override ;
    } ;

    class EasyClickMid : public BindedFuncCreator<EasyClickMid> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit EasyClickMid() ;
        virtual ~EasyClickMid() noexcept ;

        EasyClickMid(const EasyClickMid&)            = delete ;
        EasyClickMid& operator=(const EasyClickMid&) = delete ;

        EasyClickMid(EasyClickMid&&) ;
        EasyClickMid& operator=(EasyClickMid&&) ;

        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        void reconstruct() override ;
    } ;

    class EasyClickHover : public BindedFuncCreator<EasyClickHover> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit EasyClickHover() ;
        virtual ~EasyClickHover() noexcept ;

        EasyClickHover(const EasyClickHover&)            = delete ;
        EasyClickHover& operator=(const EasyClickHover&) = delete ;

        EasyClickHover(EasyClickHover&&) ;
        EasyClickHover& operator=(EasyClickHover&&) ;

        void sprocess() const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        void reconstruct() override ;
    } ;
}

#endif
