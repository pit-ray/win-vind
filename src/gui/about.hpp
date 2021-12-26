#ifndef _ABOUT_HPP
#define _ABOUT_HPP

#include "util/disable_compiler_warning.hpp"

#include <wx/dialog.h>

#include "util/enable_compiler_warning.hpp"

#include <memory>

#include "tray.hpp"


namespace vind
{
    namespace gui
    {
        class AboutDialog : public wxDialog {
        private:
            std::unique_ptr<SystemTray> systray_ ;

        public:
            explicit AboutDialog(
                    const std::string& iconpath,
                    const std::string& tooltips,
                    int font_size,
                    const std::string& font_name) ;

            virtual ~AboutDialog() noexcept ;

            AboutDialog(const AboutDialog&)            = delete ;
            AboutDialog& operator=(const AboutDialog&) = delete ;
            AboutDialog(AboutDialog&&)                 = delete ;
            AboutDialog& operator=(AboutDialog&&)      = delete ;
        } ;
    }
}

#endif
