#ifndef _G_MAPS_HPP
#define _G_MAPS_HPP

#include "mapdefs.hpp"
#include "mode.hpp"
#include <memory>

namespace vind
{
    namespace core 
    {
        enum class MapType : unsigned char {
            UNDEFINED           = 0b0000'0000,
            MAP                 = 0b0001'0000,
            NOREMAP             = 0b0010'0000,
            NOREMAP_FUNCTION    = 0b0010'0001,
        } ;

        class UniqueMap {
        private:
            struct Impl ;
            std::shared_ptr<Impl> pimpl ;

        public:
            explicit UniqueMap() ;
            explicit UniqueMap(
                    const std::string& in,
                    const std::string& out,
                    MapType expect_type,
                    bool check_if_func=false) ;

            bool is_noremap() const noexcept ;
            bool is_noremap_function() const noexcept ;
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

            bool operator==(UniqueMap&& rhs) const ;
            bool operator==(const UniqueMap& rhs) const ;

            bool operator!=(UniqueMap&& rhs) const ;
            bool operator!=(const UniqueMap& rhs) const ;
        } ;


        void initialize_maps() ;
        void reset_all_maps() ;

        void do_map(
                const std::string& incmd,
                const std::string& outcmd,
                Mode mode) ;

        void do_noremap(
                const std::string& incmd,
                const std::string& outcmd,
                Mode mode) ;

        void do_unmap(
                const std::string& incmd,
                Mode mode) ;

        void do_mapclear(Mode mode) ;

        UniqueMap get_map(
                const std::string& cmd,
                Mode mode) ;

        void get_maps(
                Mode mode,
                std::vector<UniqueMap>& returns) ;
    }
}

#endif
