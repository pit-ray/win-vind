#ifndef _INPUTHUB_HPP
#define _INPUTHUB_HPP

#include "cmdunit.hpp"
#include "mode.hpp"

#include <memory>
#include <string>
#include <vector>


namespace vind
{
    namespace core
    {
        class InputHub {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            explicit InputHub() ;
            virtual ~InputHub() noexcept ;

        public:
            static InputHub& get_instance() ;

            bool fetch_inputs(
                    std::vector<CmdUnit::SPtr>& fetched_inputs,
                    std::vector<std::uint16_t>& counts,
                    Mode mode=get_global_mode()) ;

            void pull_inputs(Mode mode=get_global_mode()) ;

            void forget_previous_input(Mode mode=get_global_mode()) ;

            void add_map(
                const std::string& trigger_cmd,
                const std::string& target_cmd,
                Mode mode) ;

            void add_noremap(
                const std::string& trigger_cmd,
                const std::string& target_cmd,
                Mode mode) ;

            bool remove_mapping(
                const std::string& trigger_cmd,
                Mode mode) ;

            void clear_mapping(Mode mode=Mode::UNDEFINED) ;
            void apply_mapping(Mode mode=Mode::UNDEFINED) ;
        } ;
    }
}

#endif
