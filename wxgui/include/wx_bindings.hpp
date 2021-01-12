#ifndef _WX_BINDINGS_HPP
#define _WX_BINDINGS_HPP

#include <memory>
#include "wx_panel_core.hpp"

namespace wxGUI
{
    class BindingsPanel : public PanelCore
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void translate() noexcept override ;
        void do_save_config() noexcept override ;
        void do_load_config() noexcept override ;

    public:
        explicit BindingsPanel(wxBookCtrlBase* const p_book_ctrl) ;
        virtual ~BindingsPanel() noexcept ;

        BindingsPanel(BindingsPanel&&) = delete ;
        BindingsPanel& operator=(BindingsPanel&&) = delete ;

        BindingsPanel(const BindingsPanel&) = delete ;
        BindingsPanel& operator=(const BindingsPanel&) = delete ;
    } ;
}


#endif
