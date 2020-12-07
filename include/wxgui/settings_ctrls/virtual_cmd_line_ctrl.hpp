#ifndef _VIRTUAL_CMD_LINE_CTRL_HPP
#define _VIRTUAL_CMD_LINE_CTRL_HPP

#include "ctrl_core.hpp"
#include <memory>

namespace wxGUI
{
    class VirtualCmdLineCtrl : public CtrlCore {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void translate() noexcept override ;
        void do_load_config() noexcept override ;
        void do_load_config_default() noexcept override ;
        void do_save_config() noexcept override ;
    public:
        explicit VirtualCmdLineCtrl(wxWindow* parent, wxWindowID id=wxID_ANY) ;

        virtual ~VirtualCmdLineCtrl() noexcept ;
        VirtualCmdLineCtrl(VirtualCmdLineCtrl&&) = delete ;
        VirtualCmdLineCtrl& operator=(VirtualCmdLineCtrl&&) = delete ;
        VirtualCmdLineCtrl(const VirtualCmdLineCtrl&) = delete ;
        VirtualCmdLineCtrl& operator=(VirtualCmdLineCtrl&) = delete ;

        const wxString name() noexcept override ;
    } ;
}

#endif
