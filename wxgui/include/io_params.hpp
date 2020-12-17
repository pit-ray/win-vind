#ifndef _IO_PARAMS_HPP
#define _IO_PARAMS_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "disable_gcc_warning.hpp"
#include <wx/string.h>
#include "enable_gcc_warning.hpp"

namespace ioParams
{
    bool load_config() ;
    bool save_config() ;

    const std::string get_vs(const std::string path) noexcept ;
    int get_vi(const std::string path) noexcept ;
    double get_vd(const std::string path) noexcept ;
    bool get_vb(const std::string path) noexcept ;
    const wxString get_label(const std::string path) noexcept ;

    using item_t = std::unordered_map<std::string, std::string> ;
    using choices_t = std::vector<item_t> ;
    const choices_t get_choices(const std::string path) noexcept ;
    void set(const std::string path, const choices_t& choices) noexcept ;

    void set(const std::string path, const wxString value) noexcept ;
    void set(const std::string path, const std::string value) noexcept ;
    void set(const std::string path, const int value) noexcept ;
    void set(const std::string path, const long value) noexcept ;
    void set(const std::string path, const double value) noexcept ;
    void set(const std::string path, const float value) noexcept ;
    void set(const std::string path, const bool value) noexcept ;

    namespace Default {
        const std::string get_vs(const std::string path) noexcept ;
        int get_vi(const std::string path) noexcept ;
        double get_vd(const std::string path) noexcept ;
        bool get_vb(const std::string path) noexcept ;
        const choices_t get_choices(const std::string path) noexcept ;
    }
}

#endif
