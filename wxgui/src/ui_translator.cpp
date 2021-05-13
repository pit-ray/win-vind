#include "ui_translator.hpp"

#include <fstream>

#include "disable_gcc_warning.hpp"

#include <nlohmann/json.hpp>

#include "enable_gcc_warning.hpp"

#include "wx_constant.hpp"

#include "err_logger.hpp"
#include "path.hpp"

#include "io_params.hpp"

namespace UITrans
{
    const wxString trans(const std::string uipath) {
        using namespace nlohmann ;
        static const auto init = [](auto&& filepath) {
            json j ;
            try {
                std::ifstream ifs(vind::path::to_u8path(std::forward<decltype(filepath)>(filepath))) ;
                ifs >> j ;
            }
            catch(const std::exception& e) {
                PRINT_ERROR(std::string(e.what()) + ", failed loading ui.json") ;
            }
            return j ;
        } ;
        static const auto j = init(vind::path::Default::UI()) ;
        static constexpr auto error_str = wxT("PRINT_ERROR") ;

        if(j.empty()) {
            return error_str ;
        }

        try {
            return wxString::FromUTF8(j.at(
                json::json_pointer("/root/" + uipath + "/" + ioParams::get_vs("ui_lang"))
            ).get<std::string>().c_str()) ;
        }
        catch(const std::exception&) {
            try {
                const auto choices = ioParams::get_choices("ui_lang") ;
                if(!choices.empty()) {
                    return wxString::FromUTF8(j.at(
                        json::json_pointer("/root/" + uipath + "/" + choices.front().at("value"))
                    ).get<std::string>().c_str()) ;
                }
            }
            catch(const std::exception& e2) {
                PRINT_ERROR(std::string(e2.what()) + ", not found") ;
                return error_str ;
            }
        }
        return error_str ;
    }
}
