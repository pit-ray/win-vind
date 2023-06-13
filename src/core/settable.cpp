#include "settable.hpp"

#include <mutex>
#include <unordered_map>
#include <vector>

namespace
{
    using namespace vind::core ;

    std::vector<Param> default_params {
        Param("hintkeys", "asdghklqwertyuiopzxcvbnmfj"),
        Param("default", "huge"),

        Param("arrangewin_ignore", ""),

        Param("autofocus_textarea", false),
        Param("autotrack_popup", false),

        Param("blockstylecaret", false),
        Param("blockstylecaret_mode", "solid"),
        Param("blockstylecaret_width", 15),

        Param("charbreak", "grapheme"),
        Param("charcache", false),

        Param("cmd_bgcolor", "323232"),
        Param("cmd_fadeout", 5),
        Param("cmd_fontcolor", "c8c8c8"),
        Param("cmd_fontextra", 1),
        Param("cmd_fontname", "Consolas"),
        Param("cmd_fontsize", 23),
        Param("cmd_fontweight", 400),
        Param("cmd_monitor", "primary"),
        Param("cmd_roughpos", "LowerLeft"),
        Param("cmd_xmargin", 32),
        Param("cmd_ymargin", 96),

        Param("cursor_accel", 90),
        Param("cursor_resolution", 250),

        Param("dedicate_to_window", false),

        Param("easyclick_bgcolor", "323232"),
        Param("easyclick_fontcolor", "c8c8c8"),
        Param("easyclick_fontname", "Consolas"),
        Param("easyclick_fontsize", 14),
        Param("easyclick_fontweight", 500),

        Param("gridmove_bgcolor", "323232"),
        Param("gridmove_fontcolor", "c8c8c8"),
        Param("gridmove_fontname", "Consolas"),
        Param("gridmove_fontsize", 14),
        Param("gridmove_fontweight", 500),
        Param("gridmove_size", "12x8"),

        Param("gui_fontname", "Segoe UI"),
        Param("gui_fontsize", 10),

        Param("hscroll_pageratio", 0.125f),
        Param("hscroll_speed", 10),

        Param("icon_style", "icon32_flat.ico"),

        Param("initmode", "i"),

        Param("jump_margin", 10),

        Param("keybrd_layout", ""),

        Param("listen_interval", 1.0f),

        Param("shell", "cmd"),
        Param("shell_startupdir", ""),
        Param("shellcmdflag", "-c"),

        Param("suppress_for_vim", false),

        Param("tempdir", "~/Downloads"),

        Param("uiacachebuild", false),
        Param("uiacachebuild_lifetime", 1500),
        Param("uiacachebuild_staybegin", 500),
        Param("uiacachebuild_stayend", 2000),

        Param("vcmdline", true),

        Param("vscroll_pageratio", 0.125f),
        Param("vscroll_speed", 30),

        Param("window_hdelta", 100),
        Param("window_vdelta", 100),
        Param("window_velocity", 100),
        Param("winresizer_initmode", 0),
    } ;
}


namespace vind
{
    namespace core
    {
        struct SetTable::Impl {
            std::unordered_map<std::string, Param> table_{} ;
            std::unordered_map<std::string, Param> deftable_{} ;
            std::mutex mtx_{} ;

            template <typename T>
            void set_from_raw(const std::string& name, T&& val) {
                std::lock_guard<std::mutex> scoped_lock{mtx_} ;
                table_[name] = Param(name, std::forward<T>(val)) ;
            }
        } ;

        SetTable::SetTable()
        : pimpl(std::make_unique<Impl>())
        {
            for(const auto& param : default_params) {
                pimpl->deftable_[param.name()] = param ;
            }
            pimpl->table_ = pimpl->deftable_ ;
        }

        SetTable::~SetTable() noexcept = default ;

        SetTable& SetTable::get_instance() {
            static SetTable instance{} ;
            return instance ;
        }

        void SetTable::clear() {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_ = pimpl->deftable_ ;
        }

        void SetTable::set(const std::string& name, bool val) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[name] = Param(name, val) ;
        }
        void SetTable::set(const std::string& name, long val) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[name] = Param(name, val) ;
        }
        void SetTable::set(const std::string& name, float val) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[name] = Param(name, val) ;
        }
        void SetTable::set(const std::string& name, std::string&& val) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[name] = Param(name, std::move(val)) ;
        }
        void SetTable::set(const std::string& name, const std::string& val) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[name] = Param(name, val) ;
        }

        void SetTable::set(const Param& param) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[param.name()] = param ;
        }
        void SetTable::set(Param&& param) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_[param.name()] = std::move(param) ;
        }

        const Param& SetTable::get(const std::string& name) const {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            return pimpl->table_.at(name) ;
        }

        void SetTable::remove(const std::string& name) {
            std::lock_guard<std::mutex> scoped_lock{pimpl->mtx_} ;
            pimpl->table_.erase(name) ;
        }
    }
}
