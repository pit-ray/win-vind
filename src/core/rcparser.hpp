#ifndef _RC_PARSER_HPP
#define _RC_PARSER_HPP

#include "mode.hpp"
#include "util/type_traits.hpp"

#include <string>
#include <utility>

namespace vind
{
    namespace core {
        //
        // It removes comments begining with double quote.
        // Example:
        //  input:  abc "str""comment
        //  output: abc "str"
        //
        void remove_dbquote_comment(std::string& line) ;

        //
        // It parses command and its argments from a line.
        // Example:
        //  input: set option_name = hoge
        //  output:
        //      first:  set
        //      second: option_name = hoge
        //
        std::pair<std::string, std::string> divide_cmd_and_args(
                const std::string& line) ;

        // 
        // It parses arguments as single (Remove the spaces on both ends).
        // Example:
        //  input:  abcd  
        //  output: abcd
        //
        std::string extract_single_arg(
                const std::string& args) ;

        // 
        // It parses arguments as double.
        // Example:
        //  input:  foo bar
        //  output:
        //      first : foo
        //      second: bar
        //
        std::pair<std::string, std::string> extract_double_args(
                const std::string& args) ;

        //
        // It parses argument as key-value syntax.
        // Example:
        //  input : foo = bar
        //  output:
        //      first : foo
        //      second: bar
        //
        std::pair<std::string, std::string> divide_key_and_value(
                const std::string& args,
                std::string delimiter="=") ;

        // 
        // It splits the string at the last occurrence of the keyword.
        // Example:
        //  input :
        //      str: nnoremap
        //      key: n
        //
        //  output:
        //      first : n
        //      second: noremap
        //
        std::pair<std::string, std::string> divide_prefix_and_cmd(
                const std::string& str,
                const std::string& split_keyword="n") ;


        enum class RunCommandsIndex : std::uint16_t {
            UNDEFINED       = 0,

            SET,
            COMMAND,
            DELCOMMAND,
            COMCLEAR,

            SOURCE,

            AUTOCMD,
            AUTOCMD_REMOVE,

            MASK_MODE = 0b0000'0000'0000'1111,

            EMPTY_MODE_MASK  = 0b0000'0000'0001'0000,
            GUI_MODE_MASK    = 0b0000'0000'0010'0000,
            EDI_MODE_MASK    = 0b0000'0000'0100'0000,
            NORMAL_MODE_MASK = 0b0000'0000'1000'0000,
            VISUAL_MODE_MASK = 0b0000'0001'0000'0000,

            MASK_MAP = 0b0001'0000'0000'0000,
            MAP      = util::enum_or(MASK_MAP, EMPTY_MODE_MASK),
            MAP_GUI  = util::enum_or(MASK_MAP, EDI_MODE_MASK),
            MAP_EDI  = util::enum_or(MASK_MAP, EDI_MODE_MASK),
            MAP_N    = util::enum_or(MASK_MAP, NORMAL_MODE_MASK),
            MAP_V    = util::enum_or(MASK_MAP, VISUAL_MODE_MASK),
            MAP_IN   = util::enum_or(MASK_MAP, static_cast<std::size_t>(Mode::INSERT)),
            MAP_GN   = util::enum_or(MASK_MAP, static_cast<std::size_t>(Mode::GUI_NORMAL)),
            MAP_GV   = util::enum_or(MASK_MAP, static_cast<std::size_t>(Mode::GUI_VISUAL)),
            MAP_EN   = util::enum_or(MASK_MAP, static_cast<std::size_t>(Mode::EDI_NORMAL)),
            MAP_EV   = util::enum_or(MASK_MAP, static_cast<std::size_t>(Mode::EDI_VISUAL)),
            MAP_RS   = util::enum_or(MASK_MAP, static_cast<std::size_t>(Mode::RESIDENT)),
            MAP_CM   = util::enum_or(MASK_MAP, static_cast<std::size_t>(Mode::COMMAND)),

            MASK_NOREMAP= 0b0010'0000'0000'0000,
            NOREMAP     = util::enum_or(MASK_NOREMAP, EMPTY_MODE_MASK),
            NOREMAP_GUI = util::enum_or(MASK_NOREMAP, GUI_MODE_MASK),
            NOREMAP_EDI = util::enum_or(MASK_NOREMAP, EDI_MODE_MASK),
            NOREMAP_N   = util::enum_or(MASK_NOREMAP, NORMAL_MODE_MASK),
            NOREMAP_V   = util::enum_or(MASK_NOREMAP, VISUAL_MODE_MASK),
            NOREMAP_IN  = util::enum_or(MASK_NOREMAP, static_cast<std::size_t>(Mode::INSERT)),
            NOREMAP_GN  = util::enum_or(MASK_NOREMAP, static_cast<std::size_t>(Mode::GUI_NORMAL)),
            NOREMAP_GV  = util::enum_or(MASK_NOREMAP, static_cast<std::size_t>(Mode::GUI_VISUAL)),
            NOREMAP_EN  = util::enum_or(MASK_NOREMAP, static_cast<std::size_t>(Mode::EDI_NORMAL)),
            NOREMAP_EV  = util::enum_or(MASK_NOREMAP, static_cast<std::size_t>(Mode::EDI_VISUAL)),
            NOREMAP_RS  = util::enum_or(MASK_NOREMAP, static_cast<std::size_t>(Mode::RESIDENT)),
            NOREMAP_CM  = util::enum_or(MASK_NOREMAP, static_cast<std::size_t>(Mode::COMMAND)),

            MASK_UNMAP = 0b0100'0000'0000'0000,
            UNMAP      = util::enum_or(MASK_UNMAP, EMPTY_MODE_MASK),
            UNMAP_GUI  = util::enum_or(MASK_UNMAP, GUI_MODE_MASK),
            UNMAP_EDI  = util::enum_or(MASK_UNMAP, EDI_MODE_MASK),
            UNMAP_N    = util::enum_or(MASK_UNMAP, NORMAL_MODE_MASK),
            UNMAP_V    = util::enum_or(MASK_UNMAP, VISUAL_MODE_MASK),
            UNMAP_IN   = util::enum_or(MASK_UNMAP, static_cast<std::size_t>(Mode::INSERT)),
            UNMAP_GN   = util::enum_or(MASK_UNMAP, static_cast<std::size_t>(Mode::GUI_NORMAL)),
            UNMAP_GV   = util::enum_or(MASK_UNMAP, static_cast<std::size_t>(Mode::GUI_VISUAL)),
            UNMAP_EN   = util::enum_or(MASK_UNMAP, static_cast<std::size_t>(Mode::EDI_NORMAL)),
            UNMAP_EV   = util::enum_or(MASK_UNMAP, static_cast<std::size_t>(Mode::EDI_VISUAL)),
            UNMAP_RS   = util::enum_or(MASK_UNMAP, static_cast<std::size_t>(Mode::RESIDENT)),
            UNMAP_CM   = util::enum_or(MASK_UNMAP, static_cast<std::size_t>(Mode::COMMAND)),

            MASK_MAPCLEAR = 0b1000'0000'0000'0000,
            MAPCLEAR      = util::enum_or(MASK_MAPCLEAR, EMPTY_MODE_MASK),
            MAPCLEAR_GUI  = util::enum_or(MASK_MAPCLEAR, GUI_MODE_MASK),
            MAPCLEAR_EDI  = util::enum_or(MASK_MAPCLEAR, EDI_MODE_MASK),
            MAPCLEAR_N    = util::enum_or(MASK_MAPCLEAR, NORMAL_MODE_MASK),
            MAPCLEAR_V    = util::enum_or(MASK_MAPCLEAR, VISUAL_MODE_MASK),
            MAPCLEAR_IN   = util::enum_or(MASK_MAPCLEAR, static_cast<std::size_t>(Mode::INSERT)),
            MAPCLEAR_GN   = util::enum_or(MASK_MAPCLEAR, static_cast<std::size_t>(Mode::GUI_NORMAL)),
            MAPCLEAR_GV   = util::enum_or(MASK_MAPCLEAR, static_cast<std::size_t>(Mode::GUI_VISUAL)),
            MAPCLEAR_EN   = util::enum_or(MASK_MAPCLEAR, static_cast<std::size_t>(Mode::EDI_NORMAL)),
            MAPCLEAR_EV   = util::enum_or(MASK_MAPCLEAR, static_cast<std::size_t>(Mode::EDI_VISUAL)),
            MAPCLEAR_RS   = util::enum_or(MASK_MAPCLEAR, static_cast<std::size_t>(Mode::RESIDENT)),
            MAPCLEAR_CM   = util::enum_or(MASK_MAPCLEAR, static_cast<std::size_t>(Mode::COMMAND)),
        } ;

        RunCommandsIndex parse_run_command(const std::string& strcmd) ;
    }
}

#endif
