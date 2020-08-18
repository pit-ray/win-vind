#ifndef _WXGUI_HPP
#define _WXGUI_HPP

#include <wx/app.h>
#include <memory>

namespace wxGUI
{
    class App : public wxApp
    {
    private:
        virtual bool OnInit() override ;
        virtual int MainLoop() override ;

    public:
        virtual ~App() noexcept ;
    } ;
}

#endif