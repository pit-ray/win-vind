#include "gridmove.hpp"

#include "hinter.hpp"

#include "util/def.hpp"
#include "util/screen_metrics.hpp"
#include "util/string.hpp"

#include "core/cmdmatcher.hpp"
#include "core/cmdunit.hpp"
#include "core/hintassign.hpp"
#include "core/settable.hpp"

#include "opt/optionlist.hpp"
#include "opt/vcmdline.hpp"

#include <string>
#include <vector>


namespace vind
{
    namespace bind
    {
        struct GridMove::Impl {
            Hinter hinter_ ;

            int grid_w_ ;
            int grid_h_ ;

            std::vector<util::Point2D> points_ ;
            std::vector<std::string> hint_texts_ ;

            std::vector<core::CmdMatcher> matchers_ ;

            Impl()
            : hinter_(opt::ref_global_options_bynames(
                    opt::VCmdLine().name())),
              grid_w_(0),
              grid_h_(0),
              points_(),
              hint_texts_(),
              matchers_()
            {}

            void assign_hints(
                std::vector<util::Point2D>& points,
                std::vector<core::Hint>& hints,
                std::vector<std::string>& hint_texts) ;
        } ;

        GridMove::GridMove()
        : BindedFuncVoid("gridmove"),
          pimpl(std::make_unique<Impl>())
        {}

        GridMove::~GridMove() noexcept = default ;
        GridMove::GridMove(GridMove&&) = default ;
        GridMove& GridMove::operator=(GridMove&&) = default ;

        void GridMove::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            pimpl->hinter_.start_matching(
                pimpl->points_, pimpl->hint_texts_, pimpl->matchers_) ;
        }

        void GridMove::reconstruct() {
            auto& settable = core::SetTable::get_instance() ;

            pimpl->hinter_.set_style(
                settable.get("gridmove_fontcolor").get<std::string>(),
                settable.get("gridmove_bgcolor").get<std::string>(),
                settable.get("gridmove_fontsize").get<long>(),
                settable.get("gridmove_fontweight").get<long>(),
                settable.get("gridmove_fontname").get<std::string>()) ;

            // splits the values (e.g., 12x8) into a width (12) and a height (8).
            auto size = util::split(
                settable.get("gridmove_size").get<std::string>(), "x") ;
            pimpl->grid_w_ = std::stoi(size[0]) ;
            pimpl->grid_h_ = std::stoi(size[1]) ;

            // Pre-computes the coordinates of the hints when some parameters are changed.
            pimpl->points_.clear() ;
            pimpl->hint_texts_.clear() ;
            std::vector<core::Hint> hints ;
            pimpl->assign_hints(pimpl->points_, hints, pimpl->hint_texts_) ;

            pimpl->matchers_.clear() ;
            for(const auto& hint : hints) {
                std::vector<core::CmdUnit::SPtr> cmds ;
                for(const auto& unit : hint) {
                    cmds.push_back(std::make_shared<core::CmdUnit>(unit)) ;
                }
                pimpl->matchers_.emplace_back(std::move(cmds)) ;
            }
        }

        void GridMove::Impl::assign_hints(
                std::vector<util::Point2D>& points,
                std::vector<core::Hint>& hints,
                std::vector<std::string>& hint_texts) {
            auto& settable = core::SetTable::get_instance() ;
            const auto hintkeys = settable.get("hintkeys").get<std::string>() ;

            auto monitors = util::get_all_monitor_metrics() ;

            // Assign hints for all monitors.
            std::vector<core::Hint> m_hints ;
            std::vector<std::string> m_hint_texts ;
            core::assign_identifier_hints(
                monitors.size(), m_hints, m_hint_texts, hintkeys) ;

            // Assign hints for grid cells of each monitor.
            std::vector<core::Hint> c_hints ;
            std::vector<std::string> c_hint_texts ;
            core::assign_identifier_hints(
                grid_h_ * grid_w_, c_hints, c_hint_texts, hintkeys) ;

            for(std::size_t mi = 0 ; mi < monitors.size() ; mi ++) {
                const auto& rect = monitors[mi].rect ;
                auto cell_w = rect.width() / grid_w_ ;
                auto cell_h = rect.height() / grid_h_ ;

                // the center of the cell at (0, 0).
                auto base_x = rect.left() + cell_w / 2 ;
                auto base_y = rect.top() + cell_h / 2 ;

                for(int ci = 0 ; ci < (grid_h_ * grid_w_) ; ci ++) {
                    points.emplace_back(
                        base_x + (ci % grid_w_) * cell_w,
                        base_y + (ci / grid_w_) * cell_h) ;

                    hint_texts.push_back(m_hint_texts[mi] + c_hint_texts[ci]) ;

                    auto hint = m_hints[mi] ;
                    hint.insert(hint.end(), c_hints[ci].begin(), c_hints[ci].end()) ;
                    hints.push_back(hint) ;
                }
            }
        }
    }
}
