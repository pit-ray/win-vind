#include "hinter.hpp"

#include "util/color.hpp"
#include "util/point2d.hpp"
#include "util/screen_textrender.hpp"
#include "util/winwrap.hpp"

#include "core/background.hpp"
#include "core/cmdmatcher.hpp"
#include "core/cmdunit.hpp"
#include "core/inputgate.hpp"
#include "core/inputhub.hpp"
#include "core/keycode.hpp"
#include "core/keycodedef.hpp"

#include <atomic>
#include <chrono>
#include <future>
#include <mutex>
#include <string>
#include <vector>


namespace
{
    using namespace vind ;

    template <typename T>
    auto expand(T&& str) {
        return " " + str + " " ;
    }

    class HintRenderer {
    private:
        util::ScreenTextRenderer painter_ ;
        std::atomic_bool running_ ;

        void draw(
                const std::vector<util::Point2D>& points,
                const std::vector<std::string>& hint_texts,
                const std::vector<std::size_t>& start_indices,
                std::vector<std::mutex>& mtxes) {
            while(running_) {
                for(int i = 0 ; i < points.size() ; i ++) {
                    std::lock_guard<std::mutex> scoped_lock(mtxes[i]) ;
                    if(start_indices[i] == std::string::npos) {
                        continue ;
                    }

                    auto text = hint_texts[i].substr(start_indices[i]) ;

                    // Do no draw unnecessarry hints.
                    if(text.empty()) {
                        continue ;
                    }

                    // If the index is greater than 0 (not first character),
                    // replace the matched one into space. (#161)
                    if(start_indices[i] > 0) {
                        text = std::string(start_indices[i], ' ') + text ;
                    }

                    painter_.draw(expand(text), points[i], 1) ;
                }
                painter_.refresh() ;
                Sleep(20) ;  // approximately 60 fps
            }
        }

    public:
        explicit HintRenderer()
        : painter_(0, 0, ""),
          running_(false)
        {}

        auto launch_drawing(
                const std::vector<util::Point2D>& points,
                const std::vector<std::string>& hint_texts,
                const std::vector<std::size_t>& start_indices,
                std::vector<std::mutex>& mtxes) {

            running_ = true ;

            auto ft = std::async(
                std::launch::async,
                &HintRenderer::draw,
                this,
                std::cref(points),
                std::cref(hint_texts),
                std::cref(start_indices),
                std::ref(mtxes)) ;
            return ft.share() ;
        }

        void stop_drawing() {
            running_ = false ;
        }

        void set_color(
                const std::string& font_color,
                const std::string& bg_color) {
            auto [tx_r, tx_g, tx_b] = util::hex2rgb(font_color) ;
            auto txcolor = RGB(tx_r, tx_g, tx_b) ;
            painter_.set_text_color(txcolor) ;

            auto [bk_r, bk_g, bk_b] = util::hex2rgb(bg_color) ;
            auto bkcolor = RGB(bk_r, bk_g, bk_b) ;
            painter_.set_back_color(bkcolor) ;
        }

        void set_font(
                long fontsize,
                long fontweight,
                const std::string& fontname) {
            painter_.set_font(fontsize, fontweight, fontname) ;
        }
    } ;

    using namespace std::chrono ;
    template <typename FutureType>
    void stop_renderer_and_wait(
            HintRenderer& renderer,
            FutureType& ft,
            std::chrono::milliseconds wait_ms=50ms) {
        // Stop drawing process and wait for finishing.
        renderer.stop_drawing() ;

        while(ft.wait_for(wait_ms) == std::future_status::timeout) {
            // Do nothing, just wait.
        }
    }
}

namespace vind
{
    namespace bind
    {
        struct Hinter::Impl
        {
            HintRenderer renderer_ ;
            core::Background bg_ ;

            template <typename T>
            Impl(T&& opts)
            : bg_(std::forward<T>(opts))
            {}
        } ;

        Hinter::Hinter(const std::vector<opt::Option::SPtr>& opts)
        : pimpl(std::make_unique<Impl>(opts))
        {}

        Hinter::Hinter(std::vector<opt::Option::SPtr>&& opts)
        : pimpl(std::make_unique<Impl>(std::move(opts)))
        {}

        Hinter::~Hinter() noexcept = default ;
        Hinter::Hinter(Hinter&&) = default ;
        Hinter& Hinter::operator=(Hinter&&) = default ;

        void Hinter::start_matching(
                const std::vector<util::Point2D>& points,
                const std::vector<std::string>& hint_texts,
                std::vector<core::CmdMatcher>& matchers) {
            auto& ihub = core::InputHub::get_instance() ;

            // If this function was called in insert/resident,
            // this instance absorbs key messages within its scope.
            core::InstantKeyAbsorber ika ;

            for(auto& mt : matchers) {
                mt.reset_state() ;
            }

            // Save matching-status as the index of texts.
            std::vector<std::size_t> start_indices(hint_texts.size(), 0) ;
            std::vector<std::mutex> mtxes(hint_texts.size()) ;

            // Start drawing process asynchronously.
            auto ft = pimpl->renderer_.launch_drawing(
                    points, hint_texts, start_indices, mtxes) ;

            while(true) {
                pimpl->bg_.update() ;

                // To use combined macros like <easyclick><click_left>,
                // get the keys from the input queue rather than the mapped queue.
                core::CmdUnit::SPtr inputs ;
                std::uint16_t count ;
                if(!ihub.get_typed_input(
                        inputs, count, core::get_global_mode(), false)) {
                    continue ;
                }

                if(inputs->is_containing(KEYCODE_ESC)
                        || inputs->is_containing(KEYCODE_ENTER)) {
                    break ;
                }

                if(inputs->is_containing(KEYCODE_BKSPACE)) {
                    // If all matchers are an initial state,
                    // the BackSpace is the same as Escape.
                    if(matchers.front().history_size() == 0) {
                        break ;
                    }

                    for(std::size_t i = 0 ; i < matchers.size() ; i ++) {
                        std::lock_guard<std::mutex> scoped_lock(mtxes[i]) ;
                        auto& mt = matchers[i] ;
                        mt.backward_state(1) ;

                        if(mt.is_rejected()) {
                            start_indices[i] = std::string::npos ;  // No more drawing
                        }
                        else {
                            start_indices[i] = mt.history_size() ; // Draw only matches
                        }
                    }

                    // Update display pixels to show back-spaced keys.
                    util::refresh_display() ;
                    continue ;
                }

                // Fetch out only the characters.
                core::CmdUnitSet ascii_set{} ;
                for(auto& key : *inputs) {
                    if(!key.is_major_system()) {
                        ascii_set.insert(key) ;
                    }
                }
                if(ascii_set.empty()) {
                    continue ;
                }
                core::CmdUnit ascii_unit(std::move(ascii_set)) ;

                bool break_loop = true ;
                for(std::size_t i = 0 ; i < matchers.size() ; i ++) {
                    std::lock_guard<std::mutex> scoped_lock(mtxes[i]) ;
                    auto& mt = matchers[i] ;
                    mt.update_state(ascii_unit) ;

                    if(mt.is_accepted()) {
                        util::set_cursor_pos(points[i]) ;
                        break ;
                    }

                    if(mt.is_rejected()) {
                        start_indices[i] = std::string::npos ; // No more drawing
                    }
                    else {
                        break_loop = false ;
                        start_indices[i] = mt.history_size() ; // Draw only matches
                    }
                }

                if(break_loop) {
                    break ;
                }
                else {
                    // If input a key, the display should be refreshed.
                    util::refresh_display() ;
                }
            }

            stop_renderer_and_wait(pimpl->renderer_, ft) ;
            util::refresh_display() ;
        }

        void Hinter::set_style(
                const std::string& font_color,
                const std::string& bg_color,
                long fontsize,
                long fontweight,
                const std::string& fontname) {
            pimpl->renderer_.set_color(font_color, bg_color) ;
            pimpl->renderer_.set_font(fontsize, fontweight, fontname) ;
        }
    }
}
