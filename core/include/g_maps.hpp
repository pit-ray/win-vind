#ifndef _G_MAPS_HPP
#define _G_MAPS_HPP

#include "mapdefs.hpp"
#include "mode.hpp"
#include <memory>

namespace vind
{
    namespace gmaps {

        class UniqueMap {
        private:
            struct Impl ;
            std::shared_ptr<Impl> pimpl ;

        public:
            explicit UniqueMap() ;
            explicit UniqueMap(
                    const std::string& in,
                    const std::string& out) ;

            explicit UniqueMap(
                    const std::string& in,
                    const std::string& out,
                    bool is_function) ;

            bool is_function() const noexcept ;

            const Command& trigger_command() const noexcept ;

            const std::string& func_name() const noexcept ;
            std::size_t func_id() const noexcept ;

            Command create_target_command() const ;

            std::size_t compute_hash() const ;
        } ;


        void initialize() ;
        void reset() ;

        void map(
                const std::string& incmd,
                const std::string& outcmd,
                mode::Mode mode) ;

        void unmap(
                const std::string& incmd,
                mode::Mode mode) ;

        void mapclear(mode::Mode mode) ;

        UniqueMap get_map(
                const std::string& cmd,
                mode::Mode mode) ;

        void get_maps(
                mode::Mode mode,
                std::vector<UniqueMap>& returns) ;
    }
}

#endif
