#ifndef _WXGUI_HPP
#define _WXGUI_HPP
#include "wxvcdef.hpp"

#include "disable_gcc_warning.hpp"
#include <wx/app.h>
#include "enable_gcc_warning.hpp"

#include <memory>

namespace wxGUI
{
    class App : public wxApp
    {
    private:
        virtual bool OnInit() override ;
        virtual int MainLoop() override ;
        virtual void OnInitCmdLine(wxCmdLineParser& parser) override ;
        virtual bool OnCmdLineParsed(wxCmdLineParser& parser) override ;

    public:
        virtual ~App() noexcept ;
    } ;
}

#endif
