#include "wx_prop_dlg.hpp"

#include <chrono>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <windows.h>

#include "disable_gcc_warning.hpp"

#include <wx/button.h>
#include <wx/choice.h>
#include <wx/defs.h>
#include <wx/font.h>
#include <wx/fontenum.h>
#include <wx/generic/stattextg.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/window.h>

#include "enable_gcc_warning.hpp"

#include "wx_bindings.hpp"
#include "wx_settings.hpp"
#include "wx_shortcut_apps.hpp"

#include "err_logger.hpp"
#include "io_params.hpp"
#include "ui_translator.hpp"
#include "wx_constant.hpp"

#include "bind/mode/change_mode.hpp"
#include "entry.hpp"
#include "key/key_absorber.hpp"
#include "mode.hpp"


namespace wxGUI
{
    using namespace UITrans ;
    using namespace vind ;
    constexpr auto APP_NAME = wxT("win-vind Config") ;

    using namespace std::chrono ;
    static constexpr auto COOL_TIME = 1s ;

    struct PropDlg::Impl {
        std::vector<PanelCore*> panels{} ;

        wxButton* ok_btn ;
        wxButton* cl_btn ;
        wxButton* ap_btn ;

        wxWindow* const parent ;

        wxFont* font ;

        explicit Impl(wxWindow* const parent_ptr)
        : panels(),
          ok_btn(nullptr),
          cl_btn(nullptr),
          ap_btn(nullptr),
          parent(parent_ptr),
          font(nullptr)
        {
            font = wxFont::New(9, wxFONTFAMILY_TELETYPE, wxFONTFLAG_DEFAULT) ;
        }

        void translate() {
            ok_btn->SetLabel(trans("buttons/ok")) ;
            cl_btn->SetLabel(trans("buttons/cancel")) ;
            ap_btn->SetLabel(trans("buttons/apply")) ;

            //create font
            font->SetPointSize(ioParams::get_vi("gui_font_size")) ;
            const auto fontname = ioParams::get_vwxs("gui_font_name") ;
            if(wxFontEnumerator().IsValidFacename(fontname)) {
                font->SetFaceName(fontname) ;
            }
            else {
                PRINT_ERROR("The font name " + fontname + " is not available.") ;
            }

            search_child_recursively(parent) ;
        }

        void search_child_recursively(wxWindow* parent_ptr) {
            for(auto node = parent_ptr->GetChildren().GetFirst() ; node ; node = node->GetNext()) {
                auto ptr = node->GetData() ;
                ptr->SetFont(*font) ;
                search_child_recursively(ptr) ;
            }
        }

        ~Impl() noexcept = default ;
        Impl(Impl&&)                 = delete ;
        Impl& operator=(Impl&&)      = delete ;
        Impl(const Impl&)            = delete ;
        Impl& operator=(const Impl&) = delete ;
    } ;

    PropDlg::PropDlg()
    : wxPropertySheetDialog(nullptr, wxID_ANY, trans("notify/preferences")),
      pimpl(std::make_unique<Impl>(this))
    {
        SetIcon(wxIcon(ioParams::get_vs("icon_style"), wxBITMAP_TYPE_ICO)) ;

        SetBackgroundColour(wxColour(*wxWHITE)) ;
        for(auto node = GetChildren().GetFirst() ; node ; node = node->GetNext()) {
            node->GetData()->SetBackgroundColour(wxColour(*wxWHITE)) ;
        }

        pimpl->panels.emplace_back(new SettingsPanel(GetBookCtrl())) ;
        pimpl->panels.emplace_back(new BindingsPanel(GetBookCtrl())) ;
        pimpl->panels.emplace_back(new ShortcutAppsPanel(GetBookCtrl())) ;

        wxSizerFlags flags ;
        flags.Border(wxALL, BORDER).Expand() ;

        auto btn_sizer = new wxBoxSizer(wxHORIZONTAL) ;

        btn_sizer->AddStretchSpacer() ;

        pimpl->ok_btn = new wxButton(this, wxID_OK,  wxT("OK"), wxDefaultPosition, wxSize(-1, -1)) ;
        btn_sizer->Add(pimpl->ok_btn, flags) ;
        pimpl->cl_btn = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxSize(-1, -1)) ;
        btn_sizer->Add(pimpl->cl_btn, flags) ;
        pimpl->ap_btn = new wxButton(this, wxID_APPLY, wxT("Apply"), wxDefaultPosition, wxSize(-1, -1)) ;
        btn_sizer->Add(pimpl->ap_btn, flags) ;

        GetInnerSizer()->Add(btn_sizer, flags) ; //The inner sizer contains the book control and button sizer.

        LayoutDialog() ;
        Centre() ;
        SetSize(wxSize(WIDTH(), HEIGHT())) ;

        load_config() ;
        save_config() ; //to arrage style

        Bind(wxEVT_BUTTON, [this](auto&) {
            save_config() ;
            load_config() ;
        }, wxID_APPLY) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            Destroy() ;
        }, wxID_OK) ;

        Bind(wxEVT_BUTTON, [this](auto&) {
            Destroy() ;
        }, wxID_CANCEL) ;

        Bind(wxEVT_CLOSE_WINDOW, [this](auto&) {
            Destroy() ;
        }) ;

        Bind(wxEVT_SIZE, [this](auto&) {
            SetSize(wxSize(WIDTH(), HEIGHT())) ; //Fix the window size
        }) ;
    }

    PropDlg::~PropDlg() noexcept = default ;

    void PropDlg::save_config() {
        for(auto& p : pimpl->panels) {
            p->save_config() ;
        }
        if(!ioParams::save_config()) {
            PRINT_ERROR("failed saving config") ;
        }
        pimpl->translate() ;
    }

    void PropDlg::load_config() {
        if(!ioParams::load_config()) {
            PRINT_ERROR(" failed loading config") ;
        }
        SetLabel(trans("notify/preferences")) ;

        pimpl->translate() ;
        for(auto& p : pimpl->panels) {
            p->load_config() ;
        }
    }
}
