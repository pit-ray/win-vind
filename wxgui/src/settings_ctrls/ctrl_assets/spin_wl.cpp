#include "spin_wl.hpp"

#include <memory>
#include <unordered_map>

#include "disable_gcc_warning.hpp"
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include "enable_gcc_warning.hpp"

#include "io_params.hpp"
#include "utility.hpp"
#include "wx_constant.hpp"

namespace wxGUI
{
    //for int
    struct SpinWithLabel::Impl {
        std::unordered_map<std::string, wxSpinCtrl*> data_int ;
        std::unordered_map<std::string, wxSpinCtrlDouble*> data_dbl ;
        std::unordered_map<std::string, wxStaticText*> label ;

        wxWindow* const parent ;
        wxSizer* sizer ;
        wxSizerFlags flags ;

        explicit Impl(wxWindow* const parent_ptr)
        : data_int(),
          data_dbl(),
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

    SpinWithLabel::SpinWithLabel(wxWindow* const parent)
    : pimpl(std::make_unique<Impl>(parent))
    {}
    SpinWithLabel::~SpinWithLabel() noexcept = default ;

    void SpinWithLabel::set_sizer(wxGridSizer* sizer, const wxSizerFlags& flags) {
        pimpl->sizer = sizer ;
        pimpl->flags = flags ;
    }

    void SpinWithLabel::create(
            const std::string name,
            const int min,
            const int max,
            const int init) {

        if(pimpl->sizer == nullptr) {
            throw LOGIC_EXCEPT("wxSizer is not registered.") ;
        }

        pimpl->label[name] = new wxStaticText(pimpl->parent, wxID_ANY, name) ;
        pimpl->sizer->Add(pimpl->label[name], pimpl->flags) ;

        pimpl->data_int[name] = new wxSpinCtrl(
            pimpl->parent, wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxSize(CTRL_WIDTH, -1),
            wxSP_ARROW_KEYS, min, max, init
        ) ;
        pimpl->sizer->Add(pimpl->data_int[name], pimpl->flags) ;
    }

    void SpinWithLabel::create(
            const std::string name,
            const double min,
            const double max,
            const double init,
            const double step) {

        if(pimpl->sizer == nullptr) {
            throw LOGIC_EXCEPT("wxSizer is not registered.") ;
        }

        pimpl->label[name] = new wxStaticText(pimpl->parent, wxID_ANY, name) ;
        pimpl->sizer->Add(pimpl->label[name], pimpl->flags) ;

        pimpl->data_dbl[name] = new wxSpinCtrlDouble(
            pimpl->parent, wxID_ANY,
            wxEmptyString, wxDefaultPosition,
            wxSize(CTRL_WIDTH, wxDefaultCoord),
            wxSP_ARROW_KEYS, min, max, init, step
        ) ;
        pimpl->sizer->Add(pimpl->data_dbl[name], pimpl->flags) ;
    }

    void SpinWithLabel::translate() {
        for(auto& l : pimpl->label) {
            l.second->SetLabel(ioParams::get_label(l.first)) ;
        }
    }

    void SpinWithLabel::load_config() {
        for(auto& p : pimpl->data_int) {
            p.second->SetValue(ioParams::get_vi(p.first)) ;
        }
        for(auto& p : pimpl->data_dbl) {
            p.second->SetValue(ioParams::get_vd(p.first)) ;
        }
    }

    void SpinWithLabel::load_config_default() {
        for(auto& p : pimpl->data_int) {
            p.second->SetValue(ioParams::Default::get_vi(p.first)) ;
        }
        for(auto& p : pimpl->data_dbl) {
            p.second->SetValue(ioParams::Default::get_vd(p.first)) ;
        }
    }

    void SpinWithLabel::save_config() {
        for(const auto& p : pimpl->data_int) {
            ioParams::set(p.first, p.second->GetValue()) ;
        }
        for(const auto& p : pimpl->data_dbl) {
            ioParams::set(p.first, p.second->GetValue()) ;
        }
    }
}
