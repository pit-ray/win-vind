#ifndef _CTRL_CORE_HPP
#define _CTRL_CORE_HPP

#include "disable_gcc_warning.hpp"
#include <wx/string.h>
#include <wx/window.h>
#include <wx/panel.h>
#include "enable_gcc_warning.hpp"

#include "wx_constant.hpp"

namespace wxGUI
{
    class CtrlCore : public wxPanel {
    private:
        virtual void do_load_config() = 0 ;
        virtual void do_save_config() = 0 ;
        virtual void do_load_config_default() = 0 ;
        virtual void translate() = 0 ;

    public:
        explicit CtrlCore(wxWindow* parent, wxWindowID id=wxID_ANY) ;
        virtual ~CtrlCore() noexcept ;
        CtrlCore(CtrlCore&&)                 = delete ;
        CtrlCore& operator=(CtrlCore&&)      = delete ;
        CtrlCore(const CtrlCore&)            = delete ;
        CtrlCore& operator=(const CtrlCore&) = delete ;

        void load_config() ;
        void load_config_default() ;
        void save_config() ;
        virtual const wxString name() = 0 ;
    } ;
}

#endif
