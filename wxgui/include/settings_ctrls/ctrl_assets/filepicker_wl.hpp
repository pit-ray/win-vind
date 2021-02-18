#ifndef _FILEPICKER_WL_HPP
#define _FILEPICKER_WL_HPP

#include "disable_gcc_warning.hpp"
#include <wx/sizer.h>
#include <wx/window.h>
#include "enable_gcc_warning.hpp"

#include <memory>
#include <string>

namespace wxGUI
{
    class FilePickerWithLabel {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        explicit FilePickerWithLabel(wxWindow* const parent) ;
        virtual ~FilePickerWithLabel() noexcept ;

        void set_sizer(wxGridSizer* sizer, const wxSizerFlags& flags) ;

        void create(const std::string name) ;

        void translate() ;
        void load_config() ;
        void load_config_default() ;
        void save_config() ;

        FilePickerWithLabel(FilePickerWithLabel&&)            = delete ;
        FilePickerWithLabel& operator=(FilePickerWithLabel&&) = delete ;
        FilePickerWithLabel(const FilePickerWithLabel&)       = delete ;
        FilePickerWithLabel& operator=(FilePickerWithLabel&)  = delete ;
    } ;
}

#endif
