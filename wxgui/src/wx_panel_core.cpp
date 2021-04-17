#include "wx_panel_core.hpp"

#include "coreio/err_logger.hpp"
#include "ui_translator.hpp"

namespace wxGUI
{
    PanelCore::PanelCore(wxBookCtrlBase* const p_book_ctrl, const std::string ui_path_code)
    : wxPanel(p_book_ctrl),
      pbk(p_book_ctrl),
      uip(std::move(ui_path_code))
    {
        if(!p_book_ctrl) {
            PRINT_ERROR("the passed wxBookCtrlBase* is nullptr") ;
            return ;
        }
        pbk->AddPage(this, wxT("PRINT_ERROR")) ;
    }
    PanelCore::~PanelCore() noexcept = default ;

    void PanelCore::trans_page() {
        const auto index = pbk->FindPage(this) ;
        if(index != wxNOT_FOUND) {
            pbk->SetPageText(index, UITrans::trans(uip)) ;
        }
    }

    void PanelCore::load_config() {
        trans_page() ;
        translate() ;
        do_load_config() ;
    }
    void PanelCore::save_config() {
        do_save_config() ;
        trans_page() ;
        translate() ;
    }
}
