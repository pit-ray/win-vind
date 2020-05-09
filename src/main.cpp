#include "msg_logger.hpp"
#include "win_notify_icon.hpp"
#include "path.hpp"
#include "ini_parser.hpp"

#include "system.hpp"

#include <memory>
#include <iostream>
#include <windows.h>

using namespace std ;

inline BOOL WINAPI CtrlHandler(DWORD) ;
inline LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM) ;

//constant variable
static const auto MAIN_SCREEN_XPOS = GetSystemMetrics(SM_CXSCREEN) ;
static const auto MAIN_SCREEN_YPOS = GetSystemMetrics(SM_CYSCREEN) ;
static constexpr auto WINDOW_WIDTH = 160 ;
static constexpr auto WINDOW_HEIGHT = 130 ;

static const auto BK_COLOR = RGB(30, 30, 30) ;
static const auto WINDOW_ALPHA = 200 ;
static const auto TX_COLOR = RGB(222, 222, 222) ;
static const auto PROJECT_NAME = "win-vind" ;
static const auto VERSION = "1.0.2-alpha" ;

//GUI ID
static constexpr auto OPTION1_ID = 0 ;
static constexpr auto OPTION2_ID = 1 ;
static constexpr auto EXIT_ID = 2 ;

//Window Handle
static HWND main_hwnd = nullptr ;
static HWND op1_hwnd = nullptr ;
static HWND op2_hwnd = nullptr ;
static HWND exit_hwnd = nullptr ;

static bool runnable = true ; //in order to prevent from being memory leak

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{
    WNDCLASSA winc ;
    winc.style = CS_HREDRAW | CS_VREDRAW ;
    winc.lpfnWndProc = WndProc ;
    winc.cbClsExtra = 0 ;
    winc.cbWndExtra = 0 ;
    winc.hInstance = hInstance ;
    winc.hIcon = LoadIcon(NULL, IDI_APPLICATION) ;
    winc.hCursor = LoadCursorA(NULL, IDC_ARROW) ;
    winc.hbrBackground = CreateSolidBrush(BK_COLOR) ;
    winc.lpszMenuName = NULL ;
    winc.lpszClassName = TEXT(PROJECT_NAME) ;

    if(!RegisterClassA(&winc)) {
        Logger::error_stream << "[Error] windows.h: " << GetLastError() << " cannot register window class.\n" ;
        return 0 ;
    }

    main_hwnd = CreateWindowEx(
        WS_EX_TOOLWINDOW | WS_EX_LAYERED,
        TEXT(PROJECT_NAME),
        TEXT(PROJECT_NAME),
        WS_POPUPWINDOW,
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL,
        hInstance, NULL
    ) ;

    if(!main_hwnd) {
        Logger::error_stream << "[Error] windows.h: " << GetLastError() << " cannot create window.\n" ;
        return 0 ;
    }

    op1_hwnd = CreateWindow(
        TEXT("BUTTON"), TEXT("Autotrack Popup"),
        WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        5, 5, WINDOW_WIDTH, 25,
        main_hwnd, reinterpret_cast<HMENU>(OPTION1_ID), hInstance, NULL
    ) ;
    if(!op1_hwnd) {
        Logger::error_stream << "[Error] windows.h: " << GetLastError() << " cannot create window.\n" ;
        return 0 ;
    }

    op2_hwnd = CreateWindow(
        TEXT("BUTTON"), TEXT("Display Command"),
        WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        5, 45, WINDOW_WIDTH, 25,
        main_hwnd, reinterpret_cast<HMENU>(OPTION2_ID), hInstance, NULL
    ) ;
    if(!op2_hwnd) {
        Logger::error_stream << "[Error] windows.h: " << GetLastError() << " cannot create window.\n" ;
        return 0 ;
    }

    exit_hwnd = CreateWindow(
        TEXT("BUTTON"), TEXT("Exit"),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        40, 80, WINDOW_WIDTH / 2, 25,
        main_hwnd, reinterpret_cast<HMENU>(EXIT_ID), hInstance, NULL
    ) ;
    if(!exit_hwnd) {
        Logger::error_stream << "[Error] windows.h: " << GetLastError() << " cannot create window.\n" ;
        return 0 ;
    }

    if(!SetLayeredWindowAttributes(main_hwnd, 0, WINDOW_ALPHA, LWA_ALPHA)) {
        Logger::error_stream << "[Error] windows.h: " << GetLastError() << " cannot set window attribute.\n" ;
        return 0 ;
    }

    //initialize check box
    const auto pt = INIParser::load_config(Path::CONFIG_OPTION_INI()) ;
    if(pt.get_optional<int>("Options.autotrack_popup").get() == 1) {
        SendMessageA(op1_hwnd, BM_SETCHECK, BST_CHECKED, 0) ;
    }
    if(pt.get_optional<int>("Options.display_cmd").get() == 1) {
        SendMessageA(op2_hwnd, BM_SETCHECK, BST_CHECKED, 0) ;
    }

    //install Notify Icon (Task Tray)
    NotifyIcon ni(main_hwnd, PROJECT_NAME, "resources/icon32.ico") ;

    //If we input control-command, for example Ctrl + C,  at console, this handler is called.
    if(!SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
        Logger::error_stream << "[Error] windows.h: " << GetLastError() << " cannot set console ctrl handler. (main.cpp)\n" ;
        return 0 ;
    }

    //show mouse cursor
    //When Windows was started up, cursor is hidden until move mouse by default.
    //Thus, send lowlevel move event in order to show cursor. 
    INPUT in ;
    in.type = INPUT_MOUSE ;
    in.mi.dx = 1 ;
    in.mi.dy = 1 ;
    in.mi.mouseData = 0 ;
    in.mi.dwFlags = MOUSEEVENTF_MOVE ;
    in.mi.time = 0 ;
    in.mi.dwExtraInfo = GetMessageExtraInfo() ;

    if(!SendInput(1, &in, sizeof(INPUT))) {
        Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (SendInput, MOUSEEVENTF_MOVE)\n" ;
        return 0 ;
    }

    if(!System::is_init()) {
        return 0 ;
    }

    MSG msg ;
    while(runnable && System::is_update()) {

        //if other window is clicked, this window is hided.
        if(GetForegroundWindow() != main_hwnd) {
            ShowWindow(main_hwnd, SW_HIDE) ;
        }
        //MessageRoop
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg) ;

            DispatchMessage(&msg) ;
        }

        Sleep(5) ; //5ms
    }

    return static_cast<int>(msg.wParam) ;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    //resource
    static const auto del_hmenu = [](void* hmenu) {
        if(!DestroyMenu(static_cast<HMENU>(hmenu))) {
            Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (DestroyMenu)\n" ;
        }
        hmenu = nullptr ;
    } ;
    static unique_ptr<void, decltype(del_hmenu)> hmenu(nullptr, del_hmenu) ;

    auto delete_brush = [](void* hbrush) {
        if(!DeleteObject(static_cast<HBRUSH>(hbrush))) {
            Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (DeleteObject)\n" ;
        }
        hbrush = nullptr ;
    } ;

    static unique_ptr<void, decltype(delete_brush)> bk_brush(CreateSolidBrush(BK_COLOR), delete_brush) ;
    static LOGFONTA lf ;

    //massage process
    switch(msg) {
        case WM_CREATE: {
            //default setting
            lf.lfHeight = 15 ;
            lf.lfWidth = 0 ;
            lf.lfEscapement = 0 ;
            lf.lfOrientation = 0 ;
            lf.lfWeight = FW_LIGHT ;
            lf.lfItalic = FALSE ;
            lf.lfUnderline = FALSE ;
            lf.lfStrikeOut = FALSE ;
            lf.lfCharSet = ANSI_CHARSET ;
            lf.lfOutPrecision = OUT_TT_ONLY_PRECIS ;
            lf.lfClipPrecision = CLIP_DEFAULT_PRECIS ;
            lf.lfQuality = ANTIALIASED_QUALITY ;
            lf.lfPitchAndFamily = 0 ;
            lf.lfFaceName[0] = '\0' ;

            return 0 ;
        }
        case WM_DESTROY: {
            runnable = false ;
            return 0 ;
        }

        case WM_QUIT: {
            runnable = false ;
            return 0 ;
        }

        case WM_CLOSE: {
            runnable = false ;
            return 0 ;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps ;
            auto hdc = BeginPaint(hwnd, &ps) ;
            if(!hdc) {
                Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (WM_PAINT::BeginPaint)\n" ;
                return 0 ;
            }

            auto font = CreateFontIndirectA(&lf) ;
            if(!font) {
                Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (WM_PAINT::CreateFontIndirectA)\n" ;
                return 0 ;
            }

            if(!SelectObject(hdc, font)) {
                Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (WM_PAINT::SelectObject)\n" ;
                return 0 ;
            }

            if(SetBkColor(hdc, BK_COLOR) == CLR_INVALID) {
                Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (WM_PAINT::SetBkColor)\n" ;
                return 0 ;
            }

            if(SetTextColor(hdc, TX_COLOR) == CLR_INVALID) {
                Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (WM_PAINT::SetTextColor)\n" ;
                return 0 ;
            }

            if(!TextOut(hdc, 90, 110, TEXT(VERSION), lstrlen(TEXT(VERSION)))) {
                Logger::error_stream << "[Error] windows.h: " << GetLastError() << " cannot write Text. (TextOut)\n" ;
                return 0 ;
            }

            if(!EndPaint(hwnd, &ps)) {
                Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (WM_PAINT::EndPaint)\n" ;
                return 0 ;
            }
            if(!DeleteObject(font)) {
                Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (WM_PAINT::DeleteObject)\n" ;
                return 0 ;
            }

            return 0 ;
        }

        case WM_CTLCOLORSTATIC: {
            //Color Setting of Popup window
            if(reinterpret_cast<HWND>(lparam) == op1_hwnd) {
                if(SetTextColor(reinterpret_cast<HDC>(wparam), TX_COLOR) == CLR_INVALID) {
                    Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (WM_CTLCOLORSTATIC::SetTextColor)\n" ;
                    return 0 ;
                }
                if(SetBkColor(reinterpret_cast<HDC>(wparam), BK_COLOR) == CLR_INVALID) {
                    Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (WM_CTLCOLORSTATIC::SetBkColor)\n" ;
                    return 0 ;
                }
                return reinterpret_cast<LRESULT>(bk_brush.get()) ;
            }

            if(reinterpret_cast<HWND>(lparam) == op2_hwnd) {
                if(SetTextColor(reinterpret_cast<HDC>(wparam), TX_COLOR) == CLR_INVALID) {
                    Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (WM_CTLCOLORSTATIC::SetTextColor)\n" ;
                    return 0 ;
                }
                if(SetBkColor(reinterpret_cast<HDC>(wparam), BK_COLOR) == CLR_INVALID) {
                    Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (WM_CTLCOLORSTATIC::SetBkColor)\n" ;
                    return 0 ;
                }
                return reinterpret_cast<LRESULT>(bk_brush.get()) ;
            }

            if(reinterpret_cast<HWND>(lparam) == exit_hwnd) {
                if(SetTextColor(reinterpret_cast<HDC>(wparam), TX_COLOR) == CLR_INVALID) {
                    Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (WM_CTLCOLORSTATIC::SetTextColor)\n" ;
                    return 0 ;
                }
                if(SetBkColor(reinterpret_cast<HDC>(wparam), BK_COLOR) == CLR_INVALID) {
                    Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (WM_CTLCOLORSTATIC::SetBkColor)\n" ;
                    return 0 ;
                }
                return reinterpret_cast<LRESULT>(bk_brush.get()) ;
            }

            break ;
        }

        case WM_SHOWWINDOW: {
            if(!wparam) {
                //window is closed
                using namespace boost::property_tree ;
                ptree pt ;
                if(SendMessageA(op1_hwnd, BM_GETCHECK, 0, 0) == BST_CHECKED) {
                    pt.put("Options.autotrack_popup", 1) ;
                }
                else {
                    pt.put("Options.autotrack_popup", 0) ;
                }
                if(SendMessageA(op2_hwnd, BM_GETCHECK, 0, 0) == BST_CHECKED) {
                    pt.put("Options.display_cmd", 1) ;
                }
                else {
                    pt.put("Options.display_cmd", 0) ;
                }

                write_ini(Path::CONFIG_OPTION_INI(), pt) ;

                System::load_option_config() ;
                break ;
            }

            return 0 ;
        }

        case WM_COMMAND: {
            switch(wparam) {
                case EXIT_ID: {
                    runnable = false ;
                    break ;
                }
            }
            return 0 ;
        }
        
        case APPWM_ICONNOTIFY: {
            static constexpr auto TASKBAR_WIDTH = 50 ;
            static constexpr auto TASKBAR_HEIGHT = 32 ;
            //On NotifyIcon
            switch(lparam) {
                case WM_RBUTTONUP: {
                    POINT pos ;
                    if(!GetCursorPos(&pos)) {
                        Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (APPWM_ICONNOTIFY::GetCursorPos)\n" ;
                        return 0 ;
                    }

                    //detect positon of Notify Icon.
                    //this is a simple method and will be replaced by a GUI.
                    if(pos.y <= TASKBAR_HEIGHT) {
                        //top
                        pos.y += TASKBAR_HEIGHT ;
                    }
                    else if(pos.y >= MAIN_SCREEN_YPOS - TASKBAR_HEIGHT) {
                        //bottom
                        pos.y -= (WINDOW_HEIGHT + TASKBAR_HEIGHT) ;
                    }

                    if(pos.x >= MAIN_SCREEN_XPOS - TASKBAR_WIDTH) {
                        //right
                        pos.x -= (WINDOW_WIDTH + TASKBAR_WIDTH) ;
                    }
                    else if(pos.x <= TASKBAR_WIDTH) {
                        //left
                        pos.x += TASKBAR_WIDTH ;
                    }
                    else {
                        //top or buttom
                        pos.x -= WINDOW_WIDTH ;
                    }

                    if(!SetWindowPos(hwnd, HWND_TOP, pos.x, pos.y, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW)) {
                        Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (APPWM_ICONNOTIFY::SetWindowPos)\n" ;
                        return 0 ;
                    }

                    if(!SetForegroundWindow(hwnd)) {
                        Logger::error_stream << "[Error] windows.h: " << GetLastError() << " (APPWM_ICONNOTIFY::SetForegroundWindow)\n" ;
                        return 0 ;
                    }

                    break ;
                }
            }
            return 0 ;
        }
    }

    return DefWindowProc(hwnd, msg, wparam, lparam) ;
}


inline BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
    switch(fdwCtrlType) {
        case CTRL_C_EVENT:
            runnable = false ;
            return TRUE ;

        case CTRL_CLOSE_EVENT:
            runnable = false ;
            return TRUE ;

        case CTRL_BREAK_EVENT:
            runnable = false ;
            return TRUE ;

        case CTRL_LOGOFF_EVENT:
            runnable = false ;
            return TRUE ;

        case CTRL_SHUTDOWN_EVENT:
            runnable = false ;
            return TRUE ;

        default:
            return FALSE ;
    }
}