/*
             _                  _           __
   _      __(_)___       _   __(_)___  ____/ /
  | | /| / / / __ \_____| | / / / __ \/ __  /
  | |/ |/ / / / / /_____/ |/ / / / / / /_/ /
  |__/|__/_/_/ /_/      |___/_/_/ /_/\__,_/

       - Vim Key Binder for Windows -

MIT License

Copyright (c) 2020-2022 pit-ray
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "entry.hpp"
#include "keycodedef.hpp"
#include <chrono>

#define _WIN32_WINNT_WIN10 0x0A00 //Windows 10

//for DPI support
#if !defined(WINVER)

#define WINVER        _WIN32_WINNT_WIN10
#define _WIN32_WINNT  _WIN32_WINNT_WIN10

#elif WINVER < _WIN32_WINNT_WIN10

#define WINVER        _WIN32_WINNT_WIN10
#define _WIN32_WINNT  _WIN32_WINNT_WIN10

#endif

#include <windows.h>

#if defined(__GNUC__)
#include <errhandlingapi.h>
#include <memoryapi.h>
#include <winerror.h>
#include <winnt.h>
#endif

#include <cstring>
#include <memory>


#include "background.hpp"
#include "cmdmatcher.hpp"
#include "errlogger.hpp"
#include "inputgate.hpp"
#include "inputhub.hpp"
#include "mapsolver.hpp"
#include "mode.hpp"
#include "path.hpp"
#include "settable.hpp"

#include "bind/bindinglist.hpp"
#include "bind/emu/moveinsert.hpp"
#include "bind/mode/change_mode.hpp"
#include "bind/saferepeat.hpp"
#include "bind/syscmd/source.hpp"
#include "opt/optionlist.hpp"
#include "opt/vcmdline.hpp"
#include "util/debug.hpp"
#include "util/interval_timer.hpp"
#include "util/type_traits.hpp"
#include "util/winwrap.hpp"


namespace vind
{
    namespace core
    {
        struct VindEntry::Impl {
            std::function<void()> exit_ ;

            // They are used for process communication between multiple win-vind.
            HANDLE map_ ;
            std::string memname_ ;
            std::size_t memsize_ ;

            bool subprocess_ ;

            util::IntervalTimer memread_timer_ ;

            Background bg_ ;

            template <typename ExitFuncType, typename String>
            Impl(ExitFuncType&& exitfunc, String&& memname, std::size_t memsize)
            : exit_(std::forward<ExitFuncType>(exitfunc)),
              map_(NULL),
              memname_(std::forward<String>(memname)),
              memsize_(memsize),
              subprocess_(false),
              memread_timer_(1000'000), //1 s
              bg_(opt::all_global_options())
            {}

            ~Impl() noexcept {
                if(map_) {
                    CloseHandle(map_) ;
                }
            }

            Impl(Impl&&)                 = delete ;
            Impl& operator=(Impl&&)      = delete ;
            Impl(const Impl&)            = delete ;
            Impl& operator=(const Impl&) = delete ;

            auto open_memfile_for_writing() {
                auto hmap = OpenFileMappingA(
                        FILE_MAP_WRITE,
                        FALSE,
                        memname_.c_str()) ;
                if(hmap == NULL) {
                    throw std::runtime_error("Failed to open mapped memory for writing.") ;
                }

                auto delete_handle = [](void* handle) {
                    if(handle) {
                        CloseHandle(handle) ;
                        handle = NULL ;
                    }
                } ;
                return std::unique_ptr<void, decltype(delete_handle)>(hmap, delete_handle) ;
            }

            auto read_memfile(HANDLE writing_memfile) {
                auto unmap_view = [](void* view) {
                    if(view) {
                        UnmapViewOfFile(view) ;
                        view = NULL ;
                    }
                } ;

                return std::unique_ptr<void, decltype(unmap_view)>(
                        MapViewOfFile(writing_memfile, FILE_MAP_WRITE, 0, 0, 0), unmap_view) ;
            }
        } ;

        VindEntry::VindEntry(
                const std::function<void()>& exit_func,
                const std::string& mapped_memname,
                std::size_t mapped_memsize)
        : pimpl(std::make_unique<Impl>(
                    exit_func,
                    mapped_memname,
                    mapped_memsize))
        {
            if(!std::filesystem::exists(CONFIG_PATH())) {
                std::filesystem::create_directories(CONFIG_PATH()) ;
            }

            Logger::get_instance().init("syslog_", 10, 15) ;

            pimpl->map_ = CreateFileMappingA(
                    INVALID_HANDLE_VALUE, NULL,
                    PAGE_READWRITE, 0,
                    static_cast<DWORD>(pimpl->memsize_), pimpl->memname_.c_str()) ;

            if(pimpl->map_ == NULL) {
                throw std::runtime_error("Could not create memory-mapped file.") ;
            }

            pimpl->subprocess_ = GetLastError() == ERROR_ALREADY_EXISTS ;

            if(!pimpl->subprocess_) {
                auto data = pimpl->read_memfile(pimpl->map_) ;
                std::memset(data.get(), 0, pimpl->memsize_) ;
            }
        }

        VindEntry::~VindEntry() noexcept = default ;

        bool VindEntry::is_subprocess() const noexcept {
            return pimpl->subprocess_ ;
        }

        void VindEntry::init(const std::string& func_request) {
            if(is_subprocess()) {
                if(!func_request.empty()) {
                    auto memfile = pimpl->open_memfile_for_writing() ;

                    if(auto data = pimpl->read_memfile(memfile.get())) {
                        std::memmove(data.get(), func_request.c_str(), func_request.length()) ;
                    }
                }
            }
            else {
                if(!func_request.empty()) {
                    if(auto data = pimpl->read_memfile(pimpl->map_)) {
                        std::memmove(data.get(), func_request.c_str(), func_request.length()) ;
                    }
                }
                init() ;
            }
        }

        void VindEntry::init() {
            if(is_subprocess()) {
                throw std::runtime_error("The main process is already running.") ;
            }

            /*
             * It enable DPI Awareness per monitor as per the documentation.
             * ref. https://docs.microsoft.com/en-us/windows/win32/hidpi/high-dpi-desktop-application-development-on-windows
             *
             * This fix was discussed in #54 (https://github.com/pit-ray/win-vind/issues/54).
             */
            if(!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {
                Logger::get_instance().error(
                    "Application scaling could not be set to Pre-Monitor V2. "
                    "Instead, set it to Legacy DPI Aware.") ;

                if(!SetProcessDPIAware()) {
                    throw std::runtime_error("Your system is not supported DPI.") ;
                }
            }

            /**
             * TODO: The screen turns black when the following SendInput 
             *       is enabled in debugging. Unknown cause.
             */
#if !defined(DEBUG)
            /**
             * When Windows was started up, cursor is hidden until
             * move mouse by default.  Thus, send lowlevel move 
             * event in order to show cursor.
             */
            INPUT in{} ;
            in.type           = INPUT_MOUSE ;
            in.mi.dx          = 1 ;
            in.mi.dy          = 1 ;
            in.mi.dwFlags     = MOUSEEVENTF_MOVE ;
            in.mi.dwExtraInfo = GetMessageExtraInfo() ;
            if(!SendInput(1, &in, sizeof(INPUT))) {
                PRINT_ERROR("Could not move the mouse cursor to show it.") ;
            }
#endif
            bind::Source::sprocess(1, "") ;

            reconstruct() ;

            // lower keyboard hook
            // If you use debugger, must be disable this line not to be slow.
            InputGate::get_instance().install_hook() ;

            std::unordered_map<std::string, bind::BindedFunc::SPtr> cm {
                {mode_to_prefix(Mode::EDI_NORMAL), bind::ToEdiNormal::create()},
                {mode_to_prefix(Mode::GUI_NORMAL), bind::ToGUINormal::create()},
                {mode_to_prefix(Mode::INSERT), bind::ToInsert::create()},
                {mode_to_prefix(Mode::RESIDENT), bind::ToResident::create()}
            } ;

            auto& settable = SetTable::get_instance() ;
            try {
                auto func = cm.at(settable.get("initmode").get<std::string>()) ;
                handle_system_call(func->process()) ;
            }
            catch(const std::out_of_range&) {
                handle_system_call(cm.at("i")->process()) ;
            }
        }

        void VindEntry::reconstruct() {
            InputHub::get_instance().apply_mapping() ;

            auto& settable = SetTable::get_instance() ;
            for(auto& opt : opt::all_global_options()) {
                if(settable.get(opt->name()).get<bool>()) {
                    opt->enable() ;
                }
                else {
                    opt->disable() ;
                }
            }

            for(auto& func : bind::all_global_binded_funcs()) {
                func->reconstruct() ;
            }
        }

        void VindEntry::update() {
            pimpl->bg_.update() ;

            if(pimpl->memread_timer_.is_passed()) {
                //check if received messages from another win-vind.
                if(auto data = pimpl->read_memfile(pimpl->map_)) {
                    std::string name(reinterpret_cast<const char*>(data.get())) ;
                    if(!name.empty()) {
                        if(auto func = bind::ref_global_func_byname(name)) {
                            handle_system_call(func->process()) ;
                        }
                        else {
                            PRINT_ERROR(name + " is invalid function name.") ;
                        }

                        std::memset(data.get(), 0, pimpl->memsize_) ;
                    }
                    return ;
                }
            }

            auto& ihub = InputHub::get_instance() ;
            do {
                CmdUnit::SPtr input ;
                std::uint16_t count ;
                if(!ihub.pull_input(input, count)) {
                    continue ;
                }
                handle_system_call(input->execute(count)) ;
            } while(!ihub.is_empty_queue()) ;
        }

        void VindEntry::handle_system_call(SystemCall systemcall) {
            if(util::enum_has_bits(systemcall, SystemCall::TERMINATE)) {
                pimpl->exit_() ;
            }
            if(util::enum_has_bits(systemcall, SystemCall::RECONSTRUCT)) {
                reconstruct() ;
            }
        }
    }
}
