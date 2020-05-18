#ifndef _WX_BIND_LIST_HPP
#define _WX_BIND_LIST_HPP

#include <memory>
#include <wx/panel.h>
#include <wx/bookctrl.h>

#include "pref_parser.hpp"

namespace wxGUI
{
    class BindListPanel : public wxPanel
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        class HotkeyScrWindow ;
        class CmdScrWindow ;

        void load_core(const PrefParser::ums_ums_str_t bind_list) ;
        void load_default() ;
        void load_all() ;

    public:
        explicit BindListPanel(wxBookCtrlBase* const p_book_ctrl) ;
        virtual ~BindListPanel() ;

        void save_all() ;

        BindListPanel(BindListPanel&&) = delete ;
        BindListPanel& operator=(BindListPanel&&) = delete ;

        BindListPanel(const BindListPanel&) = delete ;
        BindListPanel& operator=(const BindListPanel&) = delete ;
    } ;
}

#endif