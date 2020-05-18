#ifndef _WX_PROP_DLG_HPP
#define _WX_PROP_DLG_HPP

#include <wx/propdlg.h>
#include <memory>
#include "pref_parser.hpp"

namespace wxGUI
{
    class SystemTray ;

    class PropDlg : public wxPropertySheetDialog
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;
        std::unique_ptr<SystemTray> ptbi ;

        void save_all() ;

    public:
        explicit PropDlg() ;
        virtual ~PropDlg() ;

        PropDlg(PropDlg&& rhs) = delete ;
        PropDlg& operator=(PropDlg&& rhs) = delete ;

        PropDlg(const PropDlg&) = delete ;
        PropDlg& operator=(const PropDlg&) = delete ;
    } ;
}

#endif