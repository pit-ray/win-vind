#include "opt/blockstyle_caret.hpp"

#include "core/err_logger.hpp"
#include "core/inputgate.hpp"
#include "core/keycode_def.hpp"
#include "core/mode.hpp"
#include "core/settable.hpp"
#include "util/box_2d.hpp"
#include "util/def.hpp"
#include "util/mouse.hpp"
#include "util/point_2d.hpp"
#include "util/winwrap.hpp"

#include <future>
#include <memory>


namespace
{
    using namespace vind ;

    auto get_property() {
        DWORD val ;
        util::system_parameters_info(SPI_GETCARETWIDTH, 0, &val, 0) ;
        return val ;
    }

    void update_caret(HWND hwnd) {
        util::attach_thread_input(hwnd) ;

        hwnd = GetFocus() ;

        SendMessage(hwnd, WM_KILLFOCUS, 0, 0) ;
        SendMessage(hwnd, WM_SETFOCUS, 0, 0) ;

        util::detach_thread_input(hwnd) ;
    }

    void set_property(DWORD val) {
        util::system_parameters_info(
                SPI_SETCARETWIDTH, 0, val,
                SPIF_UPDATEINIFILE | SPIF_SENDCHANGE) ;

        if(auto hwnd = GetForegroundWindow()) {
            update_caret(hwnd) ;
        }
    }

    //
    // ############################################################################################
    // #                                                                                          #
    // #                                    For developers                                        #
    // #                                                                                          #
    // ############################################################################################
    //
    // Ideally, we should get the average or maximum width of a character in the focused text area
    // and change the caret size dynamically. As you can imagine, changing it every time the caret
    // is moved is not realistic, but we could at least change it every time the focus is given.
    //
    // I tried some ways, but could not work. So I leave with what I tried and some feature works.
    //
    // 1. Get width from device context
    //
    //    hwnd = GetFocus() ; // BTW, it needs thread attaching
    //    auto hdc = GetDC(hwnd) ;
    //
    //    (1)
    //    SIZE size ;
    //    GetTextExtentPoint32W(hdc, L"A", 1, &size) ;
    //    // size.cx is always 11.
    //
    //    GetTextExtentPoint32W is based on the logical coordinate, so I also tried next one.
    //
    //    // convert coordinates from logical to device.
    //    POINT p{size.cx, size.cy} ;
    //    LPtoDP(hdc, &p, 1) ;
    //
    //    However, the device context was mapped by MM_TEXT, so each logical unit is mapped to one device pixel.
    //
    //    auto mapping_mode = GetMapMode(hdc) ; // always returned MM_TEXT
    //
    //
    //    By the way, the other functions did not work either.
    //
    //    (2)
    //    int buf ;
    //    GetCharWidth32A(hdc, 'A', 'A', &buf);
    //    // buf is always 11.
    //
    //    (3)
    //    TEXTMETRIC txm ;
    //    GetTextMetrics(hdc, &txm) ;
    //    // txm.tmAveCharWidth is always 8
    //    // txm.tmMaxCharWidth is always 16
    //
    //    (4)
    //    ABC abc ;
    //    GetCharABCWidthsA(hdc, 'A', 'A', &abc) ;
    //    // abc.abcA is always 0
    //    // abc.abcB is always 11
    //    // abc.abcC is always 0
    //
    //    By the way, you can refer to ABC definitions at https://docs.microsoft.com/en-us/windows/win32/gdi/character-widths .
    //
    //
    // 2. Send a message to get font information.
    //
    //    auto handle = reinterpret_cast<HANDLE>(SendMessage(hwnd, WM_GETFONT, 0, 0)) ;
    //    LOGFONT lf ;
    //    GetObject(handle, sizeof(lf), &lf) ;
    //    // lf.lfFaceName // It is works only in simple text area like notepad.exe
    //    // lf.lfWidth    // However, it is always 0.
    //
    //
    // It may be possible to get it from properties using UI Automation. Another method is to use OCR
    // or object detection neural networks to get the position and size of characters as bounding boxes.
    // The method to get the font properties must also take into account the magnification of the text area.
    // The one by image processing will work well if we ignore the computation cost.
    //
#if defined(DEBUG)
    DWORD get_character_width_pixel() {
        auto hwnd = GetForegroundWindow() ;
        if(hwnd == NULL) {
            return 0 ;
        }

        util::attach_thread_input(hwnd) ;

        hwnd = GetFocus() ;

        //
        //
        // Not implemented yet
        //
        //

        auto& settable = core::SetTable::get_instance() ;

        auto width = static_cast<DWORD>(settable.get("blockstylecaret_width").get<int>()) ;

        util::detach_thread_input(hwnd) ;

        return width ;
    }
#endif
}


namespace vind
{
    namespace opt
    {
        struct BlockStyleCaret::Impl {
            DWORD sysval_ = 1 ;
            bool is_enabled_ = false ;
            std::future<void> ft_{} ;
        } ;

        BlockStyleCaret::BlockStyleCaret()
        : OptionCreator("blockstylecaret"),
          pimpl(std::make_unique<Impl>())
        {
            pimpl->sysval_ = get_property() ;
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
        }

        void BlockStyleCaret::do_disable() const {
            auto& settable = core::SetTable::get_instance() ;
            auto mode = util::A2a(
                    settable.get("blockstylecaret_mode").get<std::string>()) ;
            restore_caret_style(mode) ;
        }

        void BlockStyleCaret::restore_caret_style(const std::string& mode) const {
            if(!pimpl->is_enabled_) {
                return ;
            }

            if(mode == "solid") {
                if(pimpl->ft_.valid()) {
                    pimpl->ft_.wait() ;
                    pimpl->ft_.get() ;
                }
                pimpl->ft_ = std::async(std::launch::async, set_property, pimpl->sysval_) ;

            }
            else if(mode == "flex") {
                core::InputGate::get_instance().pushup(KEYCODE_LEFT) ;
            }

            pimpl->is_enabled_ = false ;
        }

        void BlockStyleCaret::make_caret_block_style(const std::string& mode) const {
            if(pimpl->is_enabled_) {
                return ;
            }

            if(mode == "solid") {
                auto& settable = core::SetTable::get_instance() ;
                auto width = settable.get("blockstylecaret_width").get<int>() ;
                if(pimpl->ft_.valid()) {
                    pimpl->ft_.wait() ;
                    pimpl->ft_.get() ;
                }
                pimpl->ft_ = std::async(std::launch::async, set_property, width) ;

            }
            else if(mode == "flex") {
                auto& igate = core::InputGate::get_instance() ;
                igate.pushup(KEYCODE_RIGHT) ;
                igate.pushup(KEYCODE_LSHIFT, KEYCODE_LEFT) ;
            }

            pimpl->is_enabled_ = true ;
        }

        void BlockStyleCaret::do_process() const {
            auto& settable = core::SetTable::get_instance() ;
            auto mode = util::A2a(
                    settable.get("blockstylecaret_mode").get<std::string>()) ;

            if(core::get_global_mode() != core::Mode::EDI_NORMAL) {
                restore_caret_style(mode) ;
                return ;
            }

            if(mode == "solid") {
                make_caret_block_style(mode) ;
            }
            else if(mode == "flex") {
                if(!core::InputGate::get_instance().pressed_list().empty()) {
                    restore_caret_style(mode) ;
                }
                else {
                    make_caret_block_style(mode) ;
                }
            }
        }
    }
}
