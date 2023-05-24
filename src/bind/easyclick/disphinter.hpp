#ifndef _DISPLAY_HINTER_HPP
#define _DISPLAY_HINTER_HPP

#include <memory>
#include <string>
#include <vector>

#include "util/point2d.hpp"

namespace vind
{
    namespace bind
    {
        class DisplayHinter {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit DisplayHinter() ;
            virtual ~DisplayHinter() noexcept ;

            DisplayHinter(const DisplayHinter&)            = delete ;
            DisplayHinter& operator=(const DisplayHinter&) = delete ;

            DisplayHinter(DisplayHinter&&) ;
            DisplayHinter& operator=(DisplayHinter&&) ;

            void load_config() ;

            void paint_all_hints(
                    const std::vector<util::Point2D>& positions,
                    const std::vector<std::string>& strhints) ;

            void paint_matching_hints(
                    const std::vector<util::Point2D>& positions,
                    const std::vector<std::string>& strhints,
                    const std::vector<unsigned char>& matched_counts) ;
        } ;
    }
}

#endif
