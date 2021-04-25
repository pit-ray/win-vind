#include "wx_settings.hpp"

#include <map>
#include <vector>

#include "disable_gcc_warning.hpp"

#include <wx/button.h>
#include <wx/colour.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/string.h>

#include "enable_gcc_warning.hpp"

#include "common_ctrl.hpp"
#include "ctrl_core.hpp"
#include "easy_click_ctrl.hpp"
#include "move_cursor_ctrl.hpp"
#include "options_ctrl.hpp"
#include "resize_window_ctrl.hpp"
#include "scroll_page_ctrl.hpp"
#include "virtual_cmd_line_ctrl.hpp"

#include "coreio/err_logger.hpp"
#include "io_params.hpp"
#include "ui_translator.hpp"
#include "wx_constant.hpp"

namespace wxGUI
{
    namespace SettingsEvt {
        enum : unsigned int {
            DEFAULT = 10100,
            SELECT_ITEM,
        } ;
    }

    struct SettingsPanel::Impl
    {
        wxListBox* list{} ;
        std::vector<CtrlCore*> ctrls{} ;
        CtrlCore* shown_ctrl = nullptr ;

        wxSizer* ctrls_sizer = nullptr ;
        wxButton* def_btn = nullptr ;

        void update_label() {
            def_btn->SetLabel(UITrans::trans("buttons/default")) ;
        }

        void switch_ctrls() noexcept {
            const auto index = list->GetSelection() ;
            if(index != wxNOT_FOUND) {
                shown_ctrl->Hide() ;
                shown_ctrl = ctrls[index] ;
                shown_ctrl->Show() ;
                ctrls_sizer->Layout() ; //resize sizer
                shown_ctrl->Fit() ; //fit to new size
            }
        }

        void update_list() {
            list->Clear() ;
            for(const auto& l : ctrls) {
                list->Append(l->name()) ;
            }
            list->SetSelection(0) ;
            switch_ctrls() ;
        }
    } ;

    SettingsPanel::SettingsPanel(wxBookCtrlBase* const p_book_ctrl)
    : PanelCore(p_book_ctrl, "notify/preferences/settings"),
      pimpl(std::make_unique<Impl>())
    {
        using namespace UITrans ;
        auto root_sizer = new wxBoxSizer(wxHORIZONTAL) ;
        pimpl->list = new wxListBox(this, SettingsEvt::SELECT_ITEM, wxDefaultPosition,
            wxSize(static_cast<int>(WIDTH() * 0.2), HEIGHT()),
            wxArrayString{}, wxLB_SINGLE
        ) ;
        root_sizer->Add(
                pimpl->list, 0,
                static_cast<int>(wxALL) | static_cast<int>(wxALIGN_CENTER),
                BORDER) ;

        pimpl->ctrls_sizer = new wxBoxSizer(wxVERTICAL) ;
        pimpl->ctrls_sizer->SetMinSize(wxSize(WIDTH() / 2, wxDefaultCoord)) ;

        std::vector<CtrlCore*> _init = {
            new CommonCtrl(this),
            new OptionsCtrl(this),
            new MoveCursorCtrl(this),
            new ScrollPageCtrl(this),
            new VirtualCmdLineCtrl(this),
            new EasyClcikCtrl(this),
            new ResizeWindowCtrl(this)
        } ;
        pimpl->ctrls = std::move(_init) ;

        for(size_t i = 0 ; i < pimpl->ctrls.size() ; i++) {
            auto& c = pimpl->ctrls[i] ;

            //fix background color (wxWidgets 3.1.3 problem)
            c->SetBackgroundColour(wxColour(*wxWHITE)) ;

            pimpl->ctrls_sizer->Add(
                    c, 0,
                    static_cast<int>(wxALL) | static_cast<int>(wxEXPAND),
                    BORDER) ;
            if(i == 0) {
                c->Show() ;
                pimpl->shown_ctrl = c ;
            }
            else {
                c->Hide() ;
            }
        }

        root_sizer->Add(
                pimpl->ctrls_sizer, 0,
                static_cast<int>(wxALL) | static_cast<int>(wxEXPAND),
                BORDER) ;
        root_sizer->AddStretchSpacer() ;

        auto def_sizer = new wxBoxSizer(wxVERTICAL) ;
        def_sizer->AddStretchSpacer() ;
        pimpl->def_btn = new wxButton(
                this, SettingsEvt::DEFAULT, wxT("Return to Default"),
                wxDefaultPosition, wxSize(-1, -1)) ;
        def_sizer->Add(
                pimpl->def_btn, 0,
                static_cast<int>(wxALL) | static_cast<int>(wxEXPAND),
                BORDER) ;
        root_sizer->Add(
                def_sizer, 0,
                static_cast<int>(wxALL) | static_cast<int>(wxEXPAND),
                BORDER) ;

        SetSizerAndFit(root_sizer) ;

        Bind(wxEVT_LISTBOX, [this](auto&) {
            pimpl->switch_ctrls() ;
        }, SettingsEvt::SELECT_ITEM) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            pimpl->shown_ctrl->load_config_default() ;
        }, SettingsEvt::DEFAULT) ;
    }

    SettingsPanel::~SettingsPanel() noexcept = default ;

    void SettingsPanel::do_load_config() {
        for(const auto& c : pimpl->ctrls) {
            c->load_config() ;
        }
    }

    void SettingsPanel::do_save_config() {
        for(const auto& c : pimpl->ctrls) {
            c->save_config() ;
        }
    }

    void SettingsPanel::translate() {
        pimpl->update_label() ;
        pimpl->update_list() ;
        Layout() ;
    }
}
