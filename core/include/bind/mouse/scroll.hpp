#ifndef _SCROLL_HPP
#define _SCROLL_HPP
#include "bind/binded_func_creator.hpp"

namespace vind
{
    class ScrollUp : public BindedFuncCreator<ScrollUp> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit ScrollUp() ;
        virtual ~ScrollUp() noexcept ;

        ScrollUp(ScrollUp&&) ;
        ScrollUp& operator=(ScrollUp&&) ;
        ScrollUp(const ScrollUp&)               = delete ;
        ScrollUp& operator=(const ScrollUp&)    = delete ;
    } ;


    class ScrollDown : public BindedFuncCreator<ScrollDown> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit ScrollDown() ;
        virtual ~ScrollDown() noexcept ;

        ScrollDown(ScrollDown&&) ;
        ScrollDown& operator=(ScrollDown&&) ;
        ScrollDown(const ScrollDown&)               = delete ;
        ScrollDown& operator=(const ScrollDown&)    = delete ;
    } ;


    class ScrollMidUp : public BindedFuncCreator<ScrollMidUp> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit ScrollMidUp() ;
        virtual ~ScrollMidUp() noexcept ;

        ScrollMidUp(ScrollMidUp&&) ;
        ScrollMidUp& operator=(ScrollMidUp&&) ;
        ScrollMidUp(const ScrollMidUp&)             = delete ;
        ScrollMidUp& operator=(const ScrollMidUp&)  = delete ;
    } ;


    class ScrollMidDown : public BindedFuncCreator<ScrollMidDown> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit ScrollMidDown() ;
        virtual ~ScrollMidDown() noexcept ;

        ScrollMidDown(ScrollMidDown&&) ;
        ScrollMidDown& operator=(ScrollMidDown&&) ;
        ScrollMidDown(const ScrollMidDown&)             = delete ;
        ScrollMidDown& operator=(const ScrollMidDown&)  = delete ;
    } ;


    class ScrollPageUp : public BindedFuncCreator<ScrollPageUp> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit ScrollPageUp() ;
        virtual ~ScrollPageUp() noexcept ;

        ScrollPageUp(ScrollPageUp&&) ;
        ScrollPageUp& operator=(ScrollPageUp&&) ;
        ScrollPageUp(const ScrollPageUp&)               = delete ;
        ScrollPageUp& operator=(const ScrollPageUp&)    = delete ;
    } ;


    class ScrollPageDown : public BindedFuncCreator<ScrollPageDown> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;


    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit ScrollPageDown() ;
        virtual ~ScrollPageDown() noexcept ;

        ScrollPageDown(ScrollPageDown&&) ;
        ScrollPageDown& operator=(ScrollPageDown&&) ;
        ScrollPageDown(const ScrollPageDown&)               = delete ;
        ScrollPageDown& operator=(const ScrollPageDown&)    = delete ;
    } ;


    class ScrollLeft : public BindedFuncCreator<ScrollLeft> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit ScrollLeft() ;
        virtual ~ScrollLeft() noexcept ;

        ScrollLeft(ScrollLeft&&) ;
        ScrollLeft& operator=(ScrollLeft&&) ;
        ScrollLeft(const ScrollLeft&)               = delete ;
        ScrollLeft& operator=(const ScrollLeft&)    = delete ;
    } ;


    class ScrollRight : public BindedFuncCreator<ScrollRight> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit ScrollRight() ;
        virtual ~ScrollRight() noexcept ;

        ScrollRight(ScrollRight&&) ;
        ScrollRight& operator=(ScrollRight&&) ;
        ScrollRight(const ScrollRight&)             = delete ;
        ScrollRight& operator=(const ScrollRight&)  = delete ;
    } ;


    class ScrollMidLeft : public BindedFuncCreator<ScrollMidLeft> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit ScrollMidLeft() ;
        virtual ~ScrollMidLeft() noexcept ;

        ScrollMidLeft(ScrollMidLeft&&) ;
        ScrollMidLeft& operator=(ScrollMidLeft&&) ;
        ScrollMidLeft(const ScrollMidLeft&)             = delete ;
        ScrollMidLeft& operator=(const ScrollMidLeft&)  = delete ;
    } ;


    class ScrollMidRight : public BindedFuncCreator<ScrollMidRight> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(unsigned int repeat_num=1) const ;
        void sprocess(NTypeLogger& parent_lgr) const ;
        void sprocess(const CharLogger& parent_lgr) const ;

        explicit ScrollMidRight() ;
        virtual ~ScrollMidRight() noexcept ;

        ScrollMidRight(ScrollMidRight&&) ;
        ScrollMidRight& operator=(ScrollMidRight&&) ;
        ScrollMidRight(const ScrollMidRight&)               = delete ;
        ScrollMidRight& operator=(const ScrollMidRight&)    = delete ;
    } ;
}
#endif
