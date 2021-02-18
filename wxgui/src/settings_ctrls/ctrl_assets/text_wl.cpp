#include "text_wl.hpp"

#include "disable_gcc_warning.hpp"
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include "enable_gcc_warning.hpp"

#include <unordered_map>

#include "wx_constant.hpp"
#include "io_params.hpp"
#include "utility.hpp"

namespace wxGUI
{
    struct TextWithLabel::Impl
    {
        std::unordered_map<std::string, wxTextCtrl*> data ;
        std::unordered_map<std::string, wxStaticText*> label ;

        wxWindow* const parent ;

        wxSizer* sizer ;
        wxSizerFlags flags ;

        Impl(wxWindow* const parent_ptr)
        : data(),
          label(),
          parent(parent_ptr),
          sizer(nullptr),
          flags()
        {}

        ~Impl() noexcept             = default ;
        Impl(Impl&&)                 = delete ;
        Impl& operator=(Impl&&)      = delete ;
        Impl(const Impl&)            = delete ;
        Impl& operator=(const Impl&) = delete ;
    } ;

    TextWithLabel::TextWithLabel(wxWindow* const parent)
    : pimpl(std::make_unique<Impl>(parent))
    {}

    TextWithLabel::~TextWithLabel() noexcept = default ;

    void TextWithLabel::set_sizer(wxGridSizer* sizer, const wxSizerFlags& flags) {
        pimpl->sizer = sizer ;
        pimpl->flags = flags ;
    }

    void TextWithLabel::create(const std::string name) {
        if(pimpl->sizer == nullptr) {
            throw LOGIC_EXCEPT("wxSizer is not registered.") ;
        }

        pimpl->label[name] = new wxStaticText(pimpl->parent, wxID_ANY, wxT("UNDEFINED")) ;
        pimpl->sizer->Add(pimpl->label[name], pimpl->flags) ;

        pimpl->data[name] = new wxTextCtrl(pimpl->parent, wxID_ANY, wxT("UNDEFINED"),
                wxDefaultPosition, wxSize(CTRL_WIDTH, -1)) ;
        pimpl->sizer->Add(pimpl->data[name], pimpl->flags) ;
    }

    void TextWithLabel::translate() {
        for(auto& l : pimpl->label) {
            l.second->SetLabel(ioParams::get_label(l.first)) ;
        }
    }

    void TextWithLabel::load_config() {
        for(auto& s : pimpl->data) {
            s.second->ChangeValue(ioParams::get_vwxs(s.first)) ;
        }
    }

    void TextWithLabel::load_config_default() {
        for(auto& s : pimpl->data) {
            s.second->ChangeValue(ioParams::Default::get_vwxs(s.first)) ;
        }
    }

    void TextWithLabel::save_config() {
        for(const auto& s : pimpl->data) {
            ioParams::set(s.first, s.second->GetLineText(0)) ;
        }
    }
}
