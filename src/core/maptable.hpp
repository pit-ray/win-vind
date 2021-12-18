/*
 *
 *

#ifndef _CONFIG_HPP
#define _CONFIG_HPP

#include "mode.hpp"

#include <memory>
#include <vector>


namespace vind
{
    namespace core
    {
        class ParamCell ;
        class MapCell ;

        class Config {
        private:
            struct Impl :
            std::unique_ptr<Impl> pimpl ;

            explicit Config() ;
            virtual ~Config() noexcept ;

        public:
            static Config& get_instance() ;

            void init() ;

            void reconstruct() ;

            void set_param(const std::string& name, int val) ;
            void set_param(const std::string& name, double val) ;
            void set_param(const std::string& name, float val) ;
            void set_param(const std::string& name, bool val) ;
            void set_param(const std::string& name, const std::string& val) ;
            void set_param(const std::string& name, std::string&& val) ;
            void set_param(ParamCell param) ;

            ParamCell get_param(const std::string& param_name) ;

            void remove_param(const std::string& param_name) ;

            class enum MapType : unsigned char {
                NOREMAP,
                FUNCTION,
                MAP,
            } ;
            void add_map(
                    const std::string& trigger_cmd,
                    const std::string& target_cmd,
                    MapType type) ;
            void add_map(MapCell) ;

            void remove_map(
                    const std::string& trigger_cmd,
                    Mode mode) ;

            void clear_map(Mode mode) ;

            MapCell get_map(
                    const std::string& trigger_cmd,
                    Mode mode) ;

            std::vector<MapCell> get_noremaps(Mode mode) ;
            std::vector<MapCell> get_maps(Mode mode) ;
        } ;

        // std::variant???
        class ParamCell {
            template <typename T>

            T get() {
            }
        } ;

        class MapCell {

        } ;
    }
}

#endif
 */
