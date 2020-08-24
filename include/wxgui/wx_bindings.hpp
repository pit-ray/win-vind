#ifndef _WX_BINDINGS_HPP
#define _WX_BINDINGS_HPP

#include <memory>
#include <wx/panel.h>
#include <wx/bookctrl.h>

namespace wxGUI
{
    class BindingsPanel : public wxPanel
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit BindingsPanel(wxBookCtrlBase* const p_book_ctrl) ;
        virtual ~BindingsPanel() noexcept ;

        void save_all() ;
        void load_all() ;

        BindingsPanel(BindingsPanel&&) = delete ;
        BindingsPanel& operator=(BindingsPanel&&) = delete ;

        BindingsPanel(const BindingsPanel&) = delete ;
        BindingsPanel& operator=(const BindingsPanel&) = delete ;
    } ;
}


#endif