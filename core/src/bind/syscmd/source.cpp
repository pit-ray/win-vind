#include "bind/syscmd/source.hpp"

#include "err_logger.hpp"
#include "key/char_logger.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "parser/rc_parser.hpp"
#include "path.hpp"
#include "util/def.hpp"

#include "bind/syscmd/command.hpp"
#include "bind/syscmd/map.hpp"
#include "bind/syscmd/set.hpp"

#include <fstream>
#include <iostream>
#include <sstream>


namespace vind
{
    SyscmdSource::SyscmdSource()
    : BindedFuncCreator("system_command_source")
    {}
    void SyscmdSource::sprocess(const std::string& path) {
        std::cout << path << std::endl ;

        std::ifstream ifs(path::to_u8path(path), std::ios::in) ;
        if(!ifs.is_open()) {
            throw RUNTIME_EXCEPT("Could not open \"" + path + "\".\n") ;
        }

        std::string aline ;
        std::size_t lnum = 0 ;
        while(getline(ifs, aline)) {
            lnum ++ ;

            auto [cmd, args] = rcparser::divide_cmd_and_args(aline) ;
            if(cmd.empty()) {
                continue ;
            }

            auto rcindex = rcparser::parse_run_command(cmd) ;

            auto error_invalid_argument = [lnum, &aline] {
                std::stringstream ss ;
                ss << "L: " << lnum ;
                VirtualCmdLine::msgout("E: Invalid Argument (L:" + ss.str() + ")") ;

                ss << ", \"" << aline << "\" invalid argument\n" ;
                throw RUNTIME_EXCEPT(ss.str()) ;
            } ;

            switch(rcindex) {
                using rcparser::RunCommandsIndex ;
                using mode::Mode ;

                case RunCommandsIndex::SET:
                    SyscmdSet::sprocess(args) ;
                    break ;


                case RunCommandsIndex::NOREMAP_GN:
                    SyscmdNoremap::sprocess(Mode::Normal, args) ;
                    break ;

                case RunCommandsIndex::NOREMAP_GV:
                    SyscmdNoremap::sprocess(Mode::Visual, args) ;
                    break ;

                case RunCommandsIndex::NOREMAP_EN:
                    SyscmdNoremap::sprocess(Mode::EdiNormal, args) ;
                    break ;

                case RunCommandsIndex::NOREMAP_EV:
                    SyscmdNoremap::sprocess(Mode::EdiVisual, args) ;
                    break ;

                case RunCommandsIndex::NOREMAP_IN:
                    SyscmdNoremap::sprocess(Mode::Insert, args) ;
                    break ;


                case RunCommandsIndex::UNMAP_GN:
                    SyscmdUnmap::sprocess(Mode::Normal, args) ;
                    break ;

                case RunCommandsIndex::UNMAP_GV:
                    SyscmdUnmap::sprocess(Mode::Visual, args) ;
                    break ;

                case RunCommandsIndex::UNMAP_EN:
                    SyscmdUnmap::sprocess(Mode::EdiNormal, args) ;
                    break ;

                case RunCommandsIndex::UNMAP_EV:
                    SyscmdUnmap::sprocess(Mode::EdiVisual, args) ;
                    break ;

                case RunCommandsIndex::UNMAP_IN:
                    SyscmdUnmap::sprocess(Mode::Insert, args) ;
                    break ;


                case RunCommandsIndex::MAPCLEAR_GN:
                    if(!args.empty()) {
                        error_invalid_argument() ;
                    }
                    SyscmdMapclear::sprocess(Mode::Normal) ;
                    break ;

                case RunCommandsIndex::MAPCLEAR_GV:
                    if(!args.empty()) {
                        error_invalid_argument() ;
                    }
                    SyscmdMapclear::sprocess(Mode::Visual) ;
                    break ;

                case RunCommandsIndex::MAPCLEAR_EN:
                    if(!args.empty()) {
                        error_invalid_argument() ;
                    }
                    SyscmdMapclear::sprocess(Mode::EdiNormal) ;
                    break ;

                case RunCommandsIndex::MAPCLEAR_EV:
                    if(!args.empty()) {
                        error_invalid_argument() ;
                    }
                    SyscmdMapclear::sprocess(Mode::EdiVisual) ;
                    break ;

                case RunCommandsIndex::MAPCLEAR_IN:
                    if(!args.empty()) {
                        error_invalid_argument() ;
                    }
                    SyscmdMapclear::sprocess(Mode::Insert) ;
                    break ;


                case RunCommandsIndex::COMMAND:
                    SyscmdCommand::sprocess(args) ;
                    break ;

                case RunCommandsIndex::DELCOMMAND:
                    SyscmdDelcommand::sprocess(args) ;
                    break ;

                case RunCommandsIndex::COMCLEAR:
                    if(!args.empty()) {
                        error_invalid_argument() ;
                    }
                    break ;

                default:
                    auto msg = "E: Invalid Syntax (L:" + std::to_string(lnum) + ")" ;
                    VirtualCmdLine::msgout(msg) ;
                    throw RUNTIME_EXCEPT(msg) ;
            } ;
        }
    }
    void SyscmdSource::sprocess(NTypeLogger&) {
    }
    void SyscmdSource::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;
        if(str.empty()) {
            throw RUNTIME_EXCEPT("Empty command") ;
        }
        auto [cmd, args] = rcparser::divide_cmd_and_args(str) ;
        if(args.empty()) {
            sprocess() ;
            return ;
        }
        sprocess(args) ;
    }
}
