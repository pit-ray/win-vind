#include "execute.hpp"

#include "openwin.hpp"

#include "core/charlogger.hpp"
#include "core/ntypelogger.hpp"
#include "core/path.hpp"
#include "core/rcparser.hpp"
#include "opt/vcmdline.hpp"
#include "util/def.hpp"
#include "util/string.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace bind
    {
        Execute::Execute()
        : BindedFuncVoid("execute")
        {}

        void Execute::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& args) {
            auto filepath = std::filesystem::u8path(util::trim(args)) ;
            if(filepath.empty()) {
                return ;
            }

            filepath = core::replace_path_magic(filepath) ;

            switch(util::shell_execute(filepath)) {
                case 0:
                case SE_ERR_OOM:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Not enough memory")) ;
                    break ;

                case ERROR_FILE_NOT_FOUND:
                case ERROR_PATH_NOT_FOUND:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Not found the file")) ;
                    break ;

                case ERROR_BAD_FORMAT:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Bad format file")) ;
                    break ;

                case SE_ERR_ACCESSDENIED:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Access is denied")) ;
                    break ;

                case SE_ERR_ASSOCINCOMPLETE:
                case SE_ERR_NOASSOC:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Association is incomplete")) ;
                    break ;

                case SE_ERR_DDEBUSY:
                    opt::VCmdLine::print(opt::ErrorMessage("E: DDE is busy")) ;
                    break ;

                case SE_ERR_DDEFAIL:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Failed DDE")) ;
                    break ;

                case SE_ERR_DDETIMEOUT:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Timeout DDE")) ;
                    break ;

                case SE_ERR_DLLNOTFOUND:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Not found DLL")) ;
                    break ;

                case SE_ERR_SHARE:
                    opt::VCmdLine::print(opt::ErrorMessage("E: Association is incomplete")) ;
                    break ;

                default:
                    break ;
            }
        }
    }
}
