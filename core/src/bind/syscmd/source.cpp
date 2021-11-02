#include "bind/syscmd/source.hpp"

#include "bind/binded_func.hpp"
#include "entry.hpp"
#include "err_logger.hpp"
#include "g_maps.hpp"
#include "g_params.hpp"
#include "key/char_logger.hpp"
#include "mode.hpp"
#include "opt/vcmdline.hpp"
#include "parser/bindings_parser.hpp"
#include "parser/rc_parser.hpp"
#include "path.hpp"
#include "util/def.hpp"

#include "bind/syscmd/command.hpp"
#include "bind/syscmd/map.hpp"
#include "bind/syscmd/set.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#if defined(DEBUG)
#include <iostream>
#endif


namespace
{
    template <typename Str>
    void do_runcommand(vind::rcparser::RunCommandsIndex rcindex, Str&& args) {
        using namespace vind ;
        using vind::rcparser::RunCommandsIndex ;

        switch(rcindex) {
            case RunCommandsIndex::SET:
                SyscmdSet::sprocess(std::forward<Str>(args), false) ;
                return ;

            case RunCommandsIndex::COMMAND:
                SyscmdCommand::sprocess(std::forward<Str>(args), false) ;
                return ;

            case RunCommandsIndex::DELCOMMAND:
                SyscmdDelcommand::sprocess(std::forward<Str>(args), false) ;
                return ;

            case RunCommandsIndex::COMCLEAR:
                if(!args.empty()) {
                    throw std::invalid_argument("comclear") ;
                }
                SyscmdComclear::sprocess(false) ;
                return ;

            case RunCommandsIndex::SOURCE:
                if(args.empty()) {
                    throw std::invalid_argument("source") ;
                }
                if(std::filesystem::equivalent(
                            path::to_u8path(path::RC()), path::to_u8path(args))) {
                    throw std::invalid_argument(
                            "Recursive references to the same .vindrc are not allowed") ;
                }
                SyscmdSource::sprocess(std::forward<Str>(args), false) ;
                return ;

            default:
                break ;
        }

        using mode::Mode ;
        auto mode = static_cast<Mode>(rcindex & RunCommandsIndex::MASK_MODE) ;

        if(rcindex & RunCommandsIndex::MASK_MAP) {
            SyscmdMap::sprocess(mode, std::forward<Str>(args), false) ;
        }
        else if(rcindex & RunCommandsIndex::MASK_NOREMAP) {
            SyscmdNoremap::sprocess(mode, std::forward<Str>(args), false) ;
        }
        else if(rcindex & RunCommandsIndex::MASK_UNMAP) {
            SyscmdUnmap::sprocess(mode, std::forward<Str>(args), false) ;
        }
        else if(rcindex & RunCommandsIndex::MASK_MAPCLEAR) {
            if(!args.empty()) {
                throw std::invalid_argument("mapclear") ;
            }
            SyscmdMapclear::sprocess(mode, false) ;
        }
        else {
            throw std::domain_error(std::to_string(static_cast<int>(rcindex))) ;
        }
    }
}


namespace vind
{
    SyscmdSource::SyscmdSource()
    : BindedFuncCreator("system_command_source")
    {
        auto f = path::to_u8path(path::RC()) ;
        std::ifstream ifs(f) ;
        if(!ifs.is_open()) {
            std::ofstream ofs(f, std::ios::trunc) ;
        }
    }

    void SyscmdSource::sprocess(
            const std::string& path,
            bool reload_config) {

        auto return_to_default = [] {
            gparams::reset() ;
            gmaps::reset() ;
        } ;

        return_to_default() ;

        std::ifstream ifs(path::to_u8path(path), std::ios::in) ;
        if(!ifs.is_open()) {
            VCmdLine::print(ErrorMessage("Could not open \"" + path + "\".\n")) ;
            return ;
        }

        std::string aline ;
        std::size_t lnum = 0 ;
        while(getline(ifs, aline)) {
            lnum ++ ;

            try {
                rcparser::remove_dbquote_comment(aline) ;

                auto [cmd, args] = rcparser::divide_cmd_and_args(aline) ;
                if(cmd.empty()) {
                    continue ;
                }

                auto rcindex = rcparser::parse_run_command(cmd) ;

                do_runcommand(rcindex, args) ;
            }
            catch(const std::domain_error& e) {
                auto ltag = "L:" + std::to_string(lnum) ;
                VCmdLine::print(ErrorMessage("E: Not command (" + ltag + ")")) ;

                std::stringstream ss ;
                ss << "RunCommandsIndex: " << e.what() << " is not supported." ;
                ss << " (" << path << ", " << ltag << ") " ;
                PRINT_ERROR(ss.str()) ;

                return_to_default() ;
                break ;
            }
            catch(const std::invalid_argument& e) {
                auto ltag = "L:" + std::to_string(lnum) ;
                VCmdLine::print(ErrorMessage("E: Invalid Argument (" + ltag + ")")) ;

                std::stringstream ss ;
                ss << e.what() << " is recieved invalid arguments." ;
                ss << " (" << path << ", " << ltag << ") " ;
                PRINT_ERROR(ss.str()) ;

                return_to_default() ;
                break ;
            }
            catch(const std::logic_error& e) {
                auto ltag = "L:" + std::to_string(lnum) ;
                VCmdLine::print(ErrorMessage("E: Invalid Syntax (" + ltag + ")")) ;

                std::stringstream ss ;
                ss << e.what() ;
                ss << " (" + path + ", " + ltag + ")" ;
                PRINT_ERROR(ss.str()) ;

                return_to_default() ;
                break ;
            }
            catch(const std::runtime_error& e) {
                auto ltag = "L:" + std::to_string(lnum) ;
                VCmdLine::print(ErrorMessage("E: Invalid Syntax (" + ltag + ")")) ;

                std::stringstream ss ;
                ss << e.what() ;
                ss << " (" + path + ", " + ltag + ")" ;
                PRINT_ERROR(ss.str()) ;

                return_to_default() ;
                break ;
            }
        }

        if(reload_config) {
            vind::reconstruct_all_components() ; // Apply settings
        }
    }
    void SyscmdSource::sprocess(NTypeLogger&) {
    }
    void SyscmdSource::sprocess(const CharLogger& parent_lgr) {
        try {
            auto str = parent_lgr.to_str() ;
            if(str.empty()) {
                throw RUNTIME_EXCEPT("Empty command") ;
            }
            auto [cmd, args] = rcparser::divide_cmd_and_args(str) ;
            if(args.empty()) {
                sprocess(path::RC(), true) ;
            }
            else {
                sprocess(args, true) ;
            }
        }
        // If received syntax error as std::logic_error,
        // convert to runtime_error not to terminate application.
        catch(const std::exception& e) {
            throw std::runtime_error(e.what()) ;
        }
    }
}
