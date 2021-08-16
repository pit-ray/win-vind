#include "opt/blockstyle_caret.hpp"

#include "err_logger.hpp"
#include "mode.hpp"
#include "util/winwrap.hpp"

#include <memory>

#if defined(DEBUG)
#include <chrono>
#include <iostream>
#endif


namespace
{
    inline auto get_property() {
        DWORD val ;
        vind::util::system_parameters_info(
                SPI_GETCARETWIDTH,
                0, &val, 0) ;
        return val ;
    }

    inline auto set_property(DWORD val) {
        vind::util::system_parameters_info(
                SPI_SETCARETWIDTH,
                0, val,
                SPIF_SENDCHANGE) ;

        //vind::util::refresh_display(NULL) ;
        auto hwnd = GetForegroundWindow() ;
        //SetForegroundWindow(NULL) ;
        //Sleep(50) ;
        //UpdateWindow(GetForegroundWindow()) ;
        //SetForegroundWindow(hwnd) ;
        //SetFocus(hwnd) ;
        /*
        if(!HideCaret(hwnd)) {
            std::cout << "not hide\n" ;
        }

        if(!ShowCaret(hwnd)) {
            std::cout << "show\n" ;
        }
        */
    }
}


namespace vind
{
    struct BlockStyleCaret::Impl {
        DWORD sysval_ = 1 ;
        DWORD flexwidth_ = 1 ;
        bool is_disabled_ = false ;
    } ;

    BlockStyleCaret::BlockStyleCaret()
    : OptionCreator("blockstylecaret"),
      pimpl(std::make_unique<Impl>())
    {
        pimpl->sysval_ = get_property() ;

        pimpl->flexwidth_ = 10 ;
    }

    BlockStyleCaret::~BlockStyleCaret() noexcept {
        try {
            if(get_property() != pimpl->sysval_) {
                set_property(pimpl->sysval_) ;
            }
        }
        catch(const std::exception& e) {
            PRINT_ERROR(e.what()) ;
        }
    }

    BlockStyleCaret::BlockStyleCaret(BlockStyleCaret&&)            = default ;
    BlockStyleCaret& BlockStyleCaret::operator=(BlockStyleCaret&&) = default ;

    void BlockStyleCaret::do_enable() const {
        set_property(pimpl->flexwidth_) ;
    }

    void BlockStyleCaret::do_disable() const {
        set_property(pimpl->sysval_) ;
    }

    void BlockStyleCaret::do_process() const {
        /*
        if(mode::get_global_mode() != mode::Mode::EDI_NORMAL) {
            if(!pimpl->is_disabled_) {
                set_property(pimpl->sysval_) ;
                pimpl->is_disabled_ = true ;
                std::cout << "Disabled\n" ;
            }
            return ;
        }

        pimpl->is_disabled_ = false ;

        using namespace std::chrono ;
        static auto n = system_clock::now() ;

        if((system_clock::now() - n) > 1s) {
            pimpl->flexwidth_ = (pimpl->flexwidth_ + 5) % 20 ;
            set_property(pimpl->flexwidth_) ;
            n = system_clock::now() ;
            std::cout << "changed to " << static_cast<int>(pimpl->flexwidth_) << std::endl ;
        }
        */
    }
}
