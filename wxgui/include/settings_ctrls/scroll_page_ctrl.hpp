#ifndef _SCROLL_PAGE_CTRL_HPP
#define _SCROLL_PAGE_CTRL_HPP

#include "ctrl_core.hpp"
#include <memory>

namespace wxGUI
{
    class ScrollPageCtrl : public CtrlCore {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void translate() noexcept override ;
        void do_load_config() noexcept override ;
        void do_load_config_default() noexcept override ;
        void do_save_config() noexcept override ;

    public:
        explicit ScrollPageCtrl(wxWindow* parent, wxWindowID id=wxID_ANY) ;

        virtual ~ScrollPageCtrl() noexcept ;
        ScrollPageCtrl(ScrollPageCtrl&&) = delete ;
        ScrollPageCtrl& operator=(ScrollPageCtrl&&) = delete ;
        ScrollPageCtrl(const ScrollPageCtrl&) = delete ;
        ScrollPageCtrl& operator=(ScrollPageCtrl&) = delete ;

        const wxString name() noexcept override ;
    } ;
}

#endif
