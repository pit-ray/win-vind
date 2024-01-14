#include "update.hpp"

#include <shlobj.h>

#include "util/disable_compiler_warning.hpp"

#include <wx/button.h>
#include <wx/defs.h>
#include <wx/fontenum.h>
#include <wx/html/htmlwin.h>
#include <wx/icon.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/sstream.h>
#include <wx/stattext.h>

#include "util/enable_compiler_warning.hpp"

#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

#include "core/errlogger.hpp"
#include "core/path.hpp"
#include "core/settable.hpp"
#include "core/version.hpp"
#include "util/def.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"

#include "util/disable_compiler_warning.hpp"

#include <maddy/parser.h>
#include <nlohmann/json.hpp>

#include "util/enable_compiler_warning.hpp"

#include "font.hpp"
#include "metrics.hpp"


// GCC
#if defined(__GNUC__)

#if defined(__x86_64__)
#define ARCHITECTURE_PREFIX "64bit"
#else
#define ARCHITECTURE_PREFIX "32bit"
#endif

// MSVC
#elif defined(_MSC_VER) && _MSC_VER >= 1500
#if defined(_WIN64)
#define ARCHITECTURE_PREFIX "64bit"
#else
#define ARCHITECTURE_PREFIX "32bit"
#endif

#endif


namespace
{
    using namespace vind ;

    enum : unsigned int {
        UPDATE_NOTES = 12000,
    } ;

    constexpr auto g_release_cache_name = "win_vind_update_checking_log.json" ;

    std::string get_asset_name_for_same_install(std::string version) {
        std::stringstream ss ;
        ss << "win-vind_" << version << "_" << ARCHITECTURE_PREFIX ;

        using core::InstallType ;

        auto install_type = core::get_install_type() ;
        switch(install_type) {
            case InstallType::INSTALLER:
                return "setup_" + ss.str() + ".exe" ;

            case InstallType::PORTABLE:
                return ss.str() + "_portable.zip" ;

            case InstallType::CHOCOLATEY:
                return ss.str() + "_chocolatey.zip" ;
        }
        throw RUNTIME_EXCEPT("Not supported install type.") ;
    }

    std::string parse_markdown_to_html(const std::string& str) {
        maddy::Parser mdparser{} ;
        std::stringstream ss ;
        ss << util::replace_all(str, "\r\n", "\n") ;
        return mdparser.Parse(ss) ;
    }
}


namespace vind
{
    namespace gui
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

            auto& settable = core::SetTable::get_instance() ;

            std::filesystem::path tempdir{
                settable.get("tempdir").get<std::string>()} ;
            tempdir = core::replace_path_magic(tempdir) ;

            util::create_process(
                tempdir,
                "curl.exe",
                util::concat_args(
                "-H", "@{\"Accept\"=\"application/vnd.github+json\"}",
                "\"https://api.github.com/repos/pit-ray/win-vind/releases/latest\"",
                "-o", g_release_cache_name), false) ;
            const auto json_path = tempdir / g_release_cache_name ;

            using namespace std::chrono ;
            auto start = system_clock::now() ;

            std::ifstream ifs ;
            while(true) {
                std::ifstream check_ifs(json_path) ;
                if(check_ifs.is_open()) {
                    ifs = std::move(check_ifs) ;
                    break ;
                }

                Sleep(500) ;

                if(system_clock::now() - start >
#if defined(DEBUG)
                        1s
#else
                        30s
#endif
                        ) { //timeout
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

            if(!ifs.is_open()) {
                util::shell_execute("https://github.com/pit-ray/win-vind/releases/latest") ;
                insert_message(wxT("Could not connect to github.com")) ;
            }
            else {
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

                        auto latest_version = tag_name.substr(1) ;
                        auto latest  = to_val_v(vind::util::split(latest_version, ".")) ;

                        if(current >= latest) {
                            insert_message(wxT("It is already the latest version.")) ;
                        }
                        else {
                            wxSizerFlags flags ;
                            flags.Border(wxALL, 10) ;
                            flags.Align(wxALIGN_CENTER_HORIZONTAL) ;

                            using core::InstallType ;
                            auto install_type = core::get_install_type() ;

                            auto body = new wxHtmlWindow(
                                    this, UPDATE_NOTES, wxDefaultPosition,
                                    get_golden_size(4), wxHW_SCROLLBAR_AUTO) ;
                            body->SetFont(*font) ;

                            std::stringstream pagess ;
                            pagess << "<html>" ;

                            // Header ==========================================
                            pagess << "<head>" ;
                            pagess << "<meta charset=\"UTF-8\">" ;
                            pagess << "</head>" ;

                            // Body =============================================
                            pagess << "<body>" ;
                            pagess << "<h1>" << tag_name << " is available now!\n</h1>" ;

                            auto release_notes = parse_markdown_to_html(js.at("body").get<std::string>()) ;
                            pagess << "<div>" << release_notes + "</div>" ;

                            if(install_type == InstallType::CHOCOLATEY) {
                                pagess <<
                                    "<br>" \
                                    "<h3>" \
                                    "The running win-vind is the Chocolatey version, " \
                                    "so use the <code>choco upgrade win-vind</code> command instead." \
                                    "<br>" \
                                    "</h3>" ;
                            }

                            // Footer ===========================================
                            pagess << "</body>" ;
                            pagess << "</html>" ;

                            body->SetPage(pagess.str()) ;
                            body->SetBorders(30) ;
                            body->SetStandardFonts(font_size, font_name) ;

                            Bind(wxEVT_HTML_LINK_CLICKED, [](wxHtmlLinkEvent& e) {
                                auto url = e.GetLinkInfo().GetHref().ToStdString() ;
                                util::shell_execute(url) ;
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

                                if(install_type == InstallType::CHOCOLATEY) {
                                    dl_btn->Enable(false) ;
                                }

                                auto cl_btn = new wxButton(this, wxID_CLOSE, wxT("Cancel")) ;
                                cl_btn->SetFont(*font) ;
                                btn_sizer->Add(cl_btn, btn_flags) ;

                                root->Add(btn_sizer, flags) ;

                                Bind(wxEVT_BUTTON, [js, latest_version, tempdir, parent] (auto&) {
                                    try {
                                        auto asset_name = get_asset_name_for_same_install(latest_version) ;

                                        for(auto& assets : js.at("assets")) {
                                            auto url = assets.at("browser_download_url").get<std::string>() ;
                                            if(url.find(asset_name) != std::string::npos) {
                                                util::create_process(
                                                        tempdir,
                                                        "curl",
                                                        util::concat_args("-OL", url), false) ;
                                                auto dl_filepath = tempdir / assets.at("name").get<std::string>() ;

                                                using namespace std::chrono ;
                                                auto dl_start = system_clock::now() ;
                                                while(true) {
                                                    Sleep(500) ;
                                                    std::ifstream check(dl_filepath) ;
                                                    if(check.is_open()) {
                                                    util::shell_execute(dl_filepath.u8string()) ;
                                                        parent->Destroy() ;  // exit win-vind for update
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
                                        util::shell_execute("https://github.com/pit-ray/win-vind/releases/latest") ;
                                    }
                                }, wxID_OK) ;
                            }
                        }
                    }
                }
                catch(const nlohmann::json::exception& e) {
                    insert_message(wxString::FromUTF8(e.what())) ;
                }
            }

            SetSizerAndFit(root) ;

            SetBackgroundColour(wxColour(*wxWHITE)) ;
            for(auto node = GetChildren().GetFirst() ; node ; node = node->GetNext()) {
                node->GetData()->SetBackgroundColour(wxColour(*wxWHITE)) ;
            }

            Bind(wxEVT_BUTTON, [this, json_path] (auto&) {
                DeleteFileW(json_path.wstring().c_str()) ;
                Destroy() ;
            }, wxID_CLOSE) ;

            Bind(wxEVT_CLOSE_WINDOW, [this, json_path](auto&) {
                DeleteFileW(json_path.wstring().c_str()) ;
                Destroy() ;
            }) ;
        }
    }
}
