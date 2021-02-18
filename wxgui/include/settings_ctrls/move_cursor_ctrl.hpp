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

        void translate() override ;
        void do_load_config() override ;
        void do_load_config_default() override ;
        void do_save_config() override ;

    public:
        explicit MoveCursorCtrl(wxWindow* parent, wxWindowID id=wxID_ANY) ;

        virtual ~MoveCursorCtrl() noexcept ;
        MoveCursorCtrl(MoveCursorCtrl&&)            = delete ;
        MoveCursorCtrl& operator=(MoveCursorCtrl&&) = delete ;
        MoveCursorCtrl(const MoveCursorCtrl&)       = delete ;
        MoveCursorCtrl& operator=(MoveCursorCtrl&)  = delete ;

        const wxString name() noexcept override ;
    } ;

}

#endif
