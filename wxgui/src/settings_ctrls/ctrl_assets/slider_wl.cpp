#include "slider_wl.hpp"

#include "disable_gcc_warning.hpp"

#include <wx/slider.h>
#include <wx/stattext.h>

#include "enable_gcc_warning.hpp"

#include <unordered_map>

#include "io_params.hpp"
#include "util/def.hpp"
#include "wx_constant.hpp"

namespace wxGUI
{
    struct SliderWithLabel::Impl
    {
        std::unordered_map<std::string, wxSlider*> data ;
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

    SliderWithLabel::SliderWithLabel(wxWindow* const parent)
    : pimpl(std::make_unique<Impl>(parent))
    {}

    SliderWithLabel::~SliderWithLabel() noexcept = default ;

    void SliderWithLabel::set_sizer(wxGridSizer* sizer, const wxSizerFlags& flags) {
        pimpl->sizer = sizer ;
        pimpl->flags = flags ;
    }

    void SliderWithLabel::create(
            const std::string name,
            const int min,
            const int max,
            const int init) {

        if(pimpl->sizer == nullptr) {
            throw LOGIC_EXCEPT("wxSizer is not registered.") ;
        }

        pimpl->label[name] = new wxStaticText(pimpl->parent, wxID_ANY, name) ;
        pimpl->sizer->Add(pimpl->label[name], pimpl->flags) ;

        pimpl->data[name] = new wxSlider(
            pimpl->parent, wxID_ANY, init, min, max,
            wxDefaultPosition, wxSize(CTRL_WIDTH, wxDefaultCoord)
        ) ;
        pimpl->sizer->Add(pimpl->data[name], pimpl->flags) ;
    }

    void SliderWithLabel::translate() {
        for(auto& l : pimpl->label) {
            l.second->SetLabel(ioParams::get_label(l.first)) ;
        }
    }

    void SliderWithLabel::load_config() {
        for(auto& s : pimpl->data) {
            s.second->SetValue(ioParams::get_vi(s.first)) ;
        }
    }

    void SliderWithLabel::load_config_default() {
        for(auto& s : pimpl->data) {
            s.second->SetValue(ioParams::Default::get_vi(s.first)) ;
        }
    }

    void SliderWithLabel::save_config() {
        for(const auto& s : pimpl->data) {
            ioParams::set(s.first, s.second->GetValue()) ;
        }
    }
}
