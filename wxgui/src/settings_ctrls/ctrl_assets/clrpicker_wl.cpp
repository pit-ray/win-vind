#include "clrpicker_wl.hpp"

#include "disable_gcc_warning.hpp"
#include <wx/stattext.h>
#include <wx/colour.h>
#include <wx/clrpicker.h>
#include "enable_gcc_warning.hpp"

#include <unordered_map>
#include <sstream>

#include "wx_constant.hpp"
#include "io_params.hpp"
#include "utility.hpp"

namespace wxGUI
{
    struct ColourPickerWithLabel::Impl
    {
        std::unordered_map<std::string, wxColourPickerCtrl*> data ;
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

    ColourPickerWithLabel::ColourPickerWithLabel(wxWindow* const parent)
    : pimpl(std::make_unique<Impl>(parent))
    {}

    ColourPickerWithLabel::~ColourPickerWithLabel() noexcept = default ;

    void ColourPickerWithLabel::set_sizer(wxGridSizer* sizer, const wxSizerFlags& flags) {
        pimpl->sizer = sizer ;
        pimpl->flags = flags ;
    }

    void ColourPickerWithLabel::create(const std::string name) {
        if(pimpl->sizer == nullptr) {
            throw LOGIC_EXCEPT("wxSizer is not registered.") ;
        }

        pimpl->label[name] = new wxStaticText(pimpl->parent, wxID_ANY, name) ;
        pimpl->sizer->Add(pimpl->label[name], pimpl->flags) ;

        pimpl->data[name] = new wxColourPickerCtrl(pimpl->parent, wxID_ANY) ;
        pimpl->sizer->Add(pimpl->data[name], pimpl->flags) ;
    }

    void ColourPickerWithLabel::translate() {
        for(auto& l : pimpl->label) {
            l.second->SetLabel(ioParams::get_label(l.first)) ;
        }
    }

    inline static const auto hex2wxColour(const std::string hex) {
        auto [r, g, b] = Utility::hex2rgb(hex) ;
        return wxColour(r, g, b) ;
    }

    inline static const auto to_hex(unsigned char val) {
        std::stringstream ss ;
        ss << std::hex << static_cast<int>(val) ;
        if(val == 0) {
            return "0" + ss.str() ;
        }
        return ss.str() ;
    }

    void ColourPickerWithLabel::load_config() {
        for(auto& p : pimpl->data) {
            p.second->SetColour(hex2wxColour(ioParams::get_vs(p.first))) ;
        }
    }

    void ColourPickerWithLabel::load_config_default() {
        for(auto& p : pimpl->data) {
            p.second->SetColour(hex2wxColour(ioParams::Default::get_vs(p.first))) ;
        }
    }

    void ColourPickerWithLabel::save_config() {
        //wxColourPickerCtrl
        for(const auto& p : pimpl->data) {
            const auto col = p.second->GetColour() ;
            auto&& chex = to_hex(col.Red()) + to_hex(col.Green()) + to_hex(col.Blue()) ;
            ioParams::set(p.first, std::move(chex)) ;
        }
    }
}
