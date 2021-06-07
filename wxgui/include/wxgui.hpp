#ifndef _WXGUI_HPP
#define _WXGUI_HPP
#include "wxvcdef.hpp"

#include "disable_gcc_warning.hpp"

#include <wx/app.h>

#include "enable_gcc_warning.hpp"

namespace wxGUI
{
    class App : public wxApp
    {
    private:
        virtual bool OnInit() override ;
        virtual bool OnExceptionInMainLoop() override ;
        virtual void OnUnhandledException() override ;
    } ;
}

#endif
