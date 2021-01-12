#ifndef _MOVE_CURSOR_HPP
#define _MOVE_CURSOR_HPP

#include "ctrl_core.hpp"
#include <memory>


namespace wxGUI
{
    class MoveCursorCtrl : public CtrlCore {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void translate() noexcept override ;
        void do_load_config() noexcept override ;
        void do_load_config_default() noexcept override ;
        void do_save_config() noexcept override ;

    public:
        explicit MoveCursorCtrl(wxWindow* parent, wxWindowID id=wxID_ANY) ;

        virtual ~MoveCursorCtrl() noexcept ;
        MoveCursorCtrl(MoveCursorCtrl&&) = delete ;
        MoveCursorCtrl& operator=(MoveCursorCtrl&&) = delete ;
        MoveCursorCtrl(const MoveCursorCtrl&) = delete ;
        MoveCursorCtrl& operator=(MoveCursorCtrl&) = delete ;

        const wxString name() noexcept override ;
    } ;

}

#endif
