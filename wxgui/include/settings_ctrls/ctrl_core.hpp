#ifndef _CTRL_CORE_HPP
#define _CTRL_CORE_HPP

#include "disable_gcc_warning.hpp"
#include <wx/string.h>
#include <wx/window.h>
#include "enable_gcc_warning.hpp"

#include "wx_constant.hpp"
#define CTRL_WIDTH static_cast<int>(WIDTH() * 0.15)

namespace wxGUI
{
    class CtrlCore : public wxWindow {
    private:
        virtual void do_load_config() noexcept = 0 ;
        virtual void do_save_config() noexcept = 0 ;
        virtual void do_load_config_default() noexcept = 0 ;
        virtual void translate() noexcept = 0 ;

    public:
        explicit CtrlCore(wxWindow* parent, wxWindowID id=wxID_ANY) ;
        virtual ~CtrlCore() noexcept ;
        CtrlCore(CtrlCore&&) = delete ;
        CtrlCore& operator=(CtrlCore&&) = delete ;
        CtrlCore(const CtrlCore&) = delete ;
        CtrlCore& operator=(const CtrlCore&) = delete ;

        void load_config() noexcept ;
        void load_config_default() noexcept ;
        void save_config() noexcept ;
        virtual const wxString name() noexcept = 0 ;
    } ;
}

#endif
