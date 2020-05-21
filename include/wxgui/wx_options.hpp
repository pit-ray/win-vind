#ifndef _WX_OPTIONS_HPP
#define _WX_OPTIONS_HPP

#include <wx/panel.h>
#include <wx/bookctrl.h>
#include <memory>

#include "pref_parser.hpp"

namespace wxGUI
{
    class OptionsPanel : public wxPanel
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void load_core(const PrefParser::ums_str_t params, const PrefParser::ums_bool_t flags) ;
        void load_default() ;

    public:
        explicit OptionsPanel(wxBookCtrlBase* const p_book_ctrl) ;
        virtual ~OptionsPanel() ;

        void save_all() ;
        void load_all() ;

        OptionsPanel(OptionsPanel&&) = delete ;
        OptionsPanel& operator=(OptionsPanel&&) = delete ;

        OptionsPanel(const OptionsPanel&) = delete ;
        OptionsPanel& operator=(const OptionsPanel&) = delete ;
    } ;
}

#endif