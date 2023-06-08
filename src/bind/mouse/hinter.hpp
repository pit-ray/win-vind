#ifndef _HINTER_HPP
#define _HINTER_HPP

#include "core/cmdmatcher.hpp"
#include "opt/option.hpp"
#include "util/point2d.hpp"

#include <memory>
#include <string>
#include <vector>


namespace vind
{
    namespace bind
    {
        class Hinter {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit Hinter(const std::vector<opt::Option::SPtr>& opts) ;
            explicit Hinter(std::vector<opt::Option::SPtr>&& opts) ;
            virtual ~Hinter() noexcept ;

            Hinter(const Hinter&) = delete ;
            Hinter& operator=(const Hinter&) = delete ;

            Hinter(Hinter&&) ;
            Hinter& operator=(Hinter&&) ;

            void start_matching(
                const std::vector<util::Point2D>& points,
                const std::vector<std::string>& hint_texts,
                std::vector<core::CmdMatcher>& matchers) ;

            void set_style(
                const std::string& font_color,
                const std::string& bg_color,
                long fontsize,
                long fontweight,
                const std::string& fontname) ;
        } ;
    }
}

#endif
