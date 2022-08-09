#ifndef _MAPTABLE_HPP
#define _MAPTABLE_HPP

#include "keycode.hpp"
#include "mode.hpp"

#include <memory>
#include <string>
#include <vector>


namespace vind
{
    namespace core
    {
        class Map ;

        class MapTable {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            explicit MapTable() ;
            virtual ~MapTable() noexcept ;

        public:
            static MapTable& get_instance() ;

            void save_asdef() ;

            void reset_todef() ;

            void add_map(
                const std::string& trigger_cmd,
                const std::string& target_cmd,
                Mode mode) ;

            void add_noremap(
                const std::string& trigger_cmd,
                const std::string& target_cmd,
                Mode mode) ;

            void add(const Map& map, Mode mode) ;

            void add(Map&& map, Mode mode) ;

            const Map& get(
                const std::string& trigger_cmd,
                Mode mode) ;

            void remove(
                const std::string& trigger_cmd,
                Mode mode) ;

            void clear(Mode mode) ;

            void clear_all() ;

            std::vector<Map> get_noremaps(Mode mode) ;

            std::vector<Map> get_remaps(Mode mode) ;

            std::vector<Map> get_allmaps(Mode mode) ;
        } ;


        class Map {
        private:
            struct Impl ;
            std::shared_ptr<Impl> pimpl ;

        public:
            Map() ;
            explicit Map(
                const std::string& in,
                const std::string& out,
                bool allow_remap=false) ;

            bool is_noremap() const noexcept ;
            bool is_map() const noexcept ;

            const Command& trigger_command() const noexcept ;
            const std::string& trigger_command_string() const noexcept ;

            const Command& target_command() const ;
            const std::string& target_command_string() const noexcept ;

            bool empty() const noexcept ;

            std::size_t in_hash() const ;
            std::size_t out_hash() const ;

            static std::size_t compute_hash(const std::string& strcmd) ;
            static std::size_t compute_hash(const Command& cmd) ;

            bool operator==(Map&& rhs) const ;
            bool operator==(const Map& rhs) const ;

            bool operator!=(Map&& rhs) const ;
            bool operator!=(const Map& rhs) const ;
        } ;
    }
}

#endif
