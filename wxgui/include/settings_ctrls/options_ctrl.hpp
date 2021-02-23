#ifndef _OPTIONS_CTRL_HPP
#define _OPTIONS_CTRL_HPP

#include <memory>
#include "ctrl_core.hpp"

namespace wxGUI
{
    class OptionsCtrl : public CtrlCore {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void translate() override ;
        void do_load_config() override ;
        void do_load_config_default() override ;
        void do_save_config() override ;

    public:
        explicit OptionsCtrl(wxWindow* parent, wxWindowID id=wxID_ANY) ;

        virtual ~OptionsCtrl() noexcept ;
        OptionsCtrl(OptionsCtrl&&)            = delete ;
        OptionsCtrl& operator=(OptionsCtrl&&) = delete ;
        OptionsCtrl(const OptionsCtrl&)       = delete ;
        OptionsCtrl& operator=(OptionsCtrl&)  = delete ;

        const wxString name() override ;
    } ;
}

#endif
