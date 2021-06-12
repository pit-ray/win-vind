#include "update.hpp"

#include <shlobj.h>

#include "disable_gcc_warning.hpp"

#include <wx/button.h>
#include <wx/defs.h>
#include <wx/fontenum.h>
#include <wx/html/htmlwin.h>
#include <wx/icon.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/sstream.h>
#include <wx/stattext.h>

#include "enable_gcc_warning.hpp"

#include <chrono>
#include <iostream>
#include <string>

#include "err_logger.hpp"
#include "font.hpp"
#include "metrics.hpp"
#include "path.hpp"
#include "util/def.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"
#include "version.hpp"

#include "bind/proc/execute.hpp"

#include "disable_gcc_warning.hpp"

#include <maddy/parser.h>
#include <nlohmann/json.hpp>

#include "enable_gcc_warning.hpp"


// GCC
#if defined(__GNUC__)

#if __x86_64__
#define INSTALLER_PREFIX "64bit"
#else
#define INSTALLER_PREFIX "32bit"
#endif

// MSVC
#elif defined(_MSC_VER) && _MSC_VER >= 1500
#if _WIN64
#define INSTALLER_PREFIX "64bit"
#else
#define INSTALLER_PREFIX "32bit"
#endif

#endif


namespace
{
    enum : unsigned int {
        UPDATE_NOTES = 12000,
    } ;

   constexpr auto g_release_json_name = "check_update_log.json" ;
   const auto g_release_json_path = vind::path::CONFIG_PATH() + "\\" + g_release_json_name ;
}


namespace vindgui
{
    UpdateDialog::UpdateDialog(
            wxWindow* parent, 
            const std::string& iconpath,
            int font_size,
            const std::string& font_name)
    : wxDialog(parent, wxID_ANY, wxT("Update win-vind"))
    {
        SetIcon(wxIcon(iconpath, wxBITMAP_TYPE_ICO)) ;
        auto root = new wxBoxSizer(wxVERTICAL) ;

        using namespace vind ;
        util::create_process(
                path::CONFIG_PATH(),
                "curl",
                util::concat_args(
                "-H", "@{\"Accept\"=\"application/vnd.github.v3+json\"}",
                "\"https://api.github.com/repos/pit-ray/win-vind/releases/latest\"",
                "-o", g_release_json_name), false) ;

        using namespace std::chrono ;
        auto start = system_clock::now() ;

        std::ifstream ifs ;
        while(true) {
            std::ifstream check_ifs(path::to_u8path(g_release_json_path)) ;
            if(check_ifs.is_open()) {
                ifs = std::move(check_ifs) ;
                break ;
            }

            Sleep(500) ;

            if(system_clock::now() - start > 30s) { //timeout
                break ;
            }
        }

        auto font = create_font(font_size, font_name) ;
        auto insert_message = [this, root, font] (auto&& msgstr) {
            wxSizerFlags flags ;
            flags.Border(wxALL, 30) ;
            flags.Align(wxALIGN_CENTER_HORIZONTAL) ;

            auto msg = new wxStaticText(this, wxID_ANY, std::forward<decltype(msgstr)>(msgstr)) ;
            msg->SetFont(*font) ;
            root->Add(msg, flags) ;

            flags.Border(wxALL, 10) ;
            auto btn = new wxButton(this, wxID_CLOSE, wxT("Close")) ;
            btn->SetFont(*font) ;
            root->Add(btn, flags) ;
        } ;

        if(ifs.is_open()) {
            nlohmann::json js ;
            ifs >> js ;
            try {
                auto tag_name = js.at("tag_name").get<std::string>() ;

                if(!tag_name.empty()) {
                    auto to_val_v = [] (auto&& mmp) {
                        return 
                        std::stoull(mmp[0]) * 1000'000 + 
                        std::stoull(mmp[1]) * 1000 +
                        std::stoull(mmp[2]) ;
                    } ;

                    auto current = to_val_v(vind::util::split(
#if defined(DEBUG)
                                "1.0.0"
#else
                                WIN_VIND_VERSION
#endif
                                , ".")) ;

                    auto latest  = to_val_v(vind::util::split(tag_name.substr(1), ".")) ;

                    if(current < latest) {
                        wxSizerFlags flags ;
                        flags.Border(wxALL, 10) ;
                        flags.Align(wxALIGN_CENTER_HORIZONTAL) ;

                        auto body = new wxHtmlWindow(
                                this, UPDATE_NOTES, wxDefaultPosition,
                                get_golden_size(4), wxHW_SCROLLBAR_AUTO) ;
                        body->SetFont(*font) ;

                        auto body_text = "# " + tag_name + " is available now!\n" + js.at("body").get<std::string>() ;
                        util::replace_all(body_text, "\r\n", "\n") ;

                        maddy::Parser mdparser{} ;
                        std::stringstream ss ;
                        ss << body_text ;

                        body->SetPage("<html><body><div>" + mdparser.Parse(ss) + "</div></body></html>") ;
                        body->SetBorders(30) ;
                        body->SetStandardFonts(font_size, font_name) ;

                        Bind(wxEVT_HTML_LINK_CLICKED, [](wxHtmlLinkEvent& e) {
                            auto url = e.GetLinkInfo().GetHref().ToStdString() ;
                            Execute::sprocess(url) ;
                        }, UPDATE_NOTES) ;

                        root->Add(body, flags) ;
                        {
                            wxSizerFlags btn_flags ;
                            btn_flags.Border(wxALL, 5) ;
                            btn_flags.Border(wxLEFT, 25) ;
                            btn_flags.Border(wxRIGHT, 25) ;
                            btn_flags.Align(wxALIGN_CENTER_VERTICAL) ;

                            auto btn_sizer = new wxBoxSizer(wxHORIZONTAL) ;
                            auto dl_btn = new wxButton(this, wxID_OK, wxT("Download")) ;
                            dl_btn->SetFont(*font) ;
                            btn_sizer->Add(dl_btn, btn_flags) ;

                            auto cl_btn = new wxButton(this, wxID_CLOSE, wxT("Cancel")) ;
                            cl_btn->SetFont(*font) ;
                            btn_sizer->Add(cl_btn, btn_flags) ;

                            root->Add(btn_sizer, flags) ;

                            Bind(wxEVT_BUTTON, [js] (auto&) {
                                try {
                                    std::string target_keyword = INSTALLER_PREFIX ;
                                    if(path::is_installer_used()) {
                                        target_keyword += ".exe.zip" ;
                                    }
                                    else {
                                        target_keyword += ".zip" ;
                                    }

                                    for(auto& assets : js.at("assets")) {
                                        auto url = assets.at("browser_download_url").get<std::string>() ;
                                        if(url.find(target_keyword) != std::string::npos) {

                                            util::create_process(
                                                    path::CONFIG_PATH(), "curl",
                                                    util::concat_args("-OL", url), false) ;

                                            auto dl_filepath = path::CONFIG_PATH() + "\\" + \
                                                               assets.at("name").get<std::string>() ;

                                            using namespace std::chrono ;
                                            auto dl_start = system_clock::now() ;
                                            while(true) {
                                                Sleep(500) ;
                                                std::ifstream check(dl_filepath) ;
                                                if(check.is_open()) {
                                                    Execute::sprocess(dl_filepath) ;
                                                    break ;
                                                }
                                                if(system_clock::now() - dl_start > 60s) { //timeout
                                                    break ;
                                                }
                                            }
                                            break ;
                                        }
                                    }
                                }
                                catch(const nlohmann::json::exception&) {
                                    Execute::sprocess(
                                            "https://github.com/pit-ray/win-vind/releases/latest") ;
                                }
                            }, wxID_OK) ;
                        }
                    }
                    else {
                        insert_message(wxT("It is already the latest version.")) ;
                    }
                }
            }
            catch(const nlohmann::json::exception& e) {
                insert_message(wxString::FromUTF8(e.what())) ;
            }
        }
        else {
            insert_message(wxT("Could not connect to github.com")) ;
        }

        SetSizerAndFit(root) ;

        SetBackgroundColour(wxColour(*wxWHITE)) ;
        for(auto node = GetChildren().GetFirst() ; node ; node = node->GetNext()) {
            node->GetData()->SetBackgroundColour(wxColour(*wxWHITE)) ;
        }

        Bind(wxEVT_BUTTON, [this] (auto&) {
            DeleteFileW(vind::util::s_to_ws(g_release_json_path).c_str()) ;
            Destroy() ;
        }, wxID_CLOSE) ;

        Bind(wxEVT_CLOSE_WINDOW, [this](auto&) {
            DeleteFileW(vind::util::s_to_ws(g_release_json_path).c_str()) ;
            Destroy() ;
        }) ;
    }
}
