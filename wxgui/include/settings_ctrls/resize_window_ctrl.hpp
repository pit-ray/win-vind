#ifndef _RESIZE_WINDOW_CTRL_HPP
#define _RESIZE_WINDOW_CTRL_HPP

#include <memory>
#include "ctrl_core.hpp"

namespace wxGUI
{
    class ResizeWindowCtrl : public CtrlCore {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void do_load_config() override ;
        void do_load_config_default() override ;
        void do_save_config() override ;
        void translate() override ;

    public:
        explicit ResizeWindowCtrl(wxWindow* parent, wxWindowID id=wxID_ANY) ;
        virtual ~ResizeWindowCtrl() noexcept ;

        ResizeWindowCtrl(ResizeWindowCtrl&&)            = delete ;
        ResizeWindowCtrl& operator=(ResizeWindowCtrl&&) = delete ;
        ResizeWindowCtrl(const ResizeWindowCtrl&)       = delete ;
        ResizeWindowCtrl& operator=(ResizeWindowCtrl&)  = delete ;

        const wxString name() override ;
    } ;
}

#endif
