#ifndef _COMMON_CTRL_HPP
#define _COMMON_CTRL_HPP

#include <memory>
#include "ctrl_core.hpp"

namespace wxGUI
{
    class CommonCtrl : public CtrlCore {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void do_load_config() override ;
        void do_load_config_default() override ;
        void do_save_config() override ;
        void translate() override ;

    public:
        explicit CommonCtrl(wxWindow* parent, wxWindowID id=wxID_ANY) ;
        virtual ~CommonCtrl() noexcept ;

        CommonCtrl(CommonCtrl&&)            = delete ;
        CommonCtrl& operator=(CommonCtrl&&) = delete ;
        CommonCtrl(const CommonCtrl&)       = delete ;
        CommonCtrl& operator=(CommonCtrl&)  = delete ;

        const wxString name() override ;
    } ;
}
#endif
