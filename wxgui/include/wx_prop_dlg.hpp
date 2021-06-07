#ifndef _WX_PROP_DLG_HPP
#define _WX_PROP_DLG_HPP
#include "wxvcdef.hpp"

#include "disable_gcc_warning.hpp"

#include <wx/propdlg.h>

#include "enable_gcc_warning.hpp"

#include <memory>

namespace wxGUI
{
    class SystemTray ;

    class PropDlg : public wxPropertySheetDialog
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void save_config() ;
        void load_config() ;

    public:
        explicit PropDlg() ;
        virtual ~PropDlg() noexcept ;

        PropDlg(PropDlg&& rhs) = delete ;
        PropDlg& operator=(PropDlg&& rhs) = delete ;

        PropDlg(const PropDlg&) = delete ;
        PropDlg& operator=(const PropDlg&) = delete ;
    } ;
}

#endif
