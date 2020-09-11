#include "ui_translator.hpp"

#include <fstream>

#include "disable_gcc_warning.hpp"
#include <nlohmann/json.hpp>
#include "enable_gcc_warning.hpp"

#include "wx_constant.hpp"
#include "msg_logger.hpp"
#include "io_params.hpp"
#include "path.hpp"

namespace UITrans
{
    const wxString trans(const std::string uipath) noexcept {
        using namespace nlohmann ;
        static const auto init = [](auto&& filepath) {
            json j ;
            try {
                std::ifstream ifs(std::forward<decltype(filepath)>(filepath)) ;
                ifs >> j ;
            }
            catch(const std::exception& e) {
                ERROR_STREAM << e.what() << ", failed loading ui.json (UITranslator::trans)\n" ;
            }
            return j ;
        } ;
        static const auto j = init(Path::Default::UI()) ;
        static constexpr auto error_str = wxT("ERROR") ;

        if(j.empty()) {
            return error_str ;
        }

        try {
            return wxString::FromUTF8(j.at(
                json::json_pointer("/root/" + uipath + "/" + ioParams::get_vs("ui_lang"))
            ).get<std::string>().c_str()) ;
        }
        catch(const std::exception& e) {
            try {
                const auto choices = ioParams::get_choices("ui_lang") ;
                if(!choices.empty()) {
                    return wxString::FromUTF8(j.at(
                        json::json_pointer("/root/" + uipath + "/" + choices.front().at("value"))
                    ).get<std::string>().c_str()) ;
                }
            }
            catch(const std::exception& e2) {
                ERROR_STREAM << e2.what() << ", not found (UITranslator::trans)\n" ;
                return error_str ;
            }
        }
        return error_str ;
    }
}
