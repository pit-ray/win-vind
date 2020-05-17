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

        class HotkeyScrWindow ;
        class CmdScrWindow ;

        void load_all(const PrefParser::ums_ums_str_t binds,
                      const PrefParser::ums_str_t params,
                      const PrefParser::ums_bool_t opts) ;
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