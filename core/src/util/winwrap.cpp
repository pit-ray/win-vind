#include "util/winwrap.hpp"

namespace vind
{
    namespace util {
        MSG get_win_message() noexcept {
            MSG msg ;
            if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg) ;
                DispatchMessage(&msg) ;
            }
            return msg ;
        }

        void refresh_display(HWND UNUSED(hwnd)) {
            //Some applications flicker by passing a NULL HWND as the first argument.  
            //This nedes improvement.
            if(!InvalidateRect(NULL, NULL, TRUE)) {
                throw RUNTIME_EXCEPT(" failed refresh display") ;
            }
        }

        bool is_existed_dir(const std::string& path)
        {
          auto flag = GetFileAttributesW(s_to_ws(path).c_str());
          return (flag != INVALID_FILE_ATTRIBUTES && (flag & FILE_ATTRIBUTE_DIRECTORY));
        }

        void create_directory(const std::string& path) {
            if(!CreateDirectoryW(s_to_ws(path).c_str(), NULL)) {
                throw RUNTIME_EXCEPT("Cannot create a directory " + path + ".") ;
            }
        }
        void copy_file(const std::string& src, const std::string& dst, bool allow_overwrite) {
            if(!CopyFileW(
                        s_to_ws(src).c_str(),
                        s_to_ws(dst).c_str(),
                        !b_to_B(allow_overwrite))) {

                throw std::runtime_error("Could not copy a file from " + src + " to " + dst + ".") ;
            }
        }
    }
}
