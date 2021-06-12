#include "bind/proc/execute.hpp"

#include "path.hpp"

#include "util/def.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"

#include "key/char_logger.hpp"
#include "opt/virtual_cmd_line.hpp"
#include "parser/rc_parser.hpp"

#include "bind/proc/open_window.hpp"


namespace vind
{
    Execute::Execute()
    : BindedFuncCreator("execute")
    {}

    void Execute::sprocess(std::string filepath) {
        if(filepath.empty()) {
            return ;
        }

        util::replace_all(filepath, "~", path::HOME_PATH()) ;

        switch(util::shell_execute(filepath)) {
            case 0:
            case SE_ERR_OOM:
                VirtualCmdLine::msgout("E: Not enough memory") ;
                break ;

            case ERROR_FILE_NOT_FOUND:
            case ERROR_PATH_NOT_FOUND:
                VirtualCmdLine::msgout("E: Not found the file") ;
                break ;

            case ERROR_BAD_FORMAT:
                VirtualCmdLine::msgout("E: Bad format file") ;
                break ;

            case SE_ERR_ACCESSDENIED:
                VirtualCmdLine::msgout("E: Access is denied") ;
                break ;

            case SE_ERR_ASSOCINCOMPLETE:
            case SE_ERR_NOASSOC:
                VirtualCmdLine::msgout("E: Association is incomplete") ;
                break ;

            case SE_ERR_DDEBUSY:
                VirtualCmdLine::msgout("E: DDE is busy") ;
                break ;

            case SE_ERR_DDEFAIL:
                VirtualCmdLine::msgout("E: Failed DDE") ;
                break ;

            case SE_ERR_DDETIMEOUT:
                VirtualCmdLine::msgout("E: Timeout DDE") ;
                break ;

            case SE_ERR_DLLNOTFOUND:
                VirtualCmdLine::msgout("E: Not found DLL") ;
                break ;

            case SE_ERR_SHARE:
                VirtualCmdLine::msgout("E: Association is incomplete") ;
                break ;

            default:
                break ;
        }
    }

    void Execute::sprocess(NTypeLogger& UNUSED(parent_lgr)) {
    }

    void Execute::sprocess(const CharLogger& parent_lgr) {
        auto str = parent_lgr.to_str() ;

        auto [cmd, arg] = rcparser::divide_cmd_and_args(str) ;
        if(arg.empty()) {
            OpenNewWindow::sprocess() ;
            return ;
        }

        sprocess(arg) ;
    }
}
