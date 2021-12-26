#ifndef _INPUT_HINTER_HPP
#define _INPUT_HINTER_HPP

#include <future>
#include <memory>

#include "easyclickhint.hpp"
#include "util/point2d.hpp"

namespace vind
{
    namespace bind
    {
        class InputHinter {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit InputHinter() ;
            virtual ~InputHinter() noexcept ;

            InputHinter(InputHinter&&)      = delete ;
            InputHinter(const InputHinter&) = delete ;
            InputHinter& operator=(InputHinter&&)      = delete ;
            InputHinter& operator=(const InputHinter&) = delete ;

            void load_config() ;

            std::shared_ptr<util::Point2D> launch_loop(
                    const std::vector<util::Point2D>& positions,
                    const std::vector<Hint>& hints) ;

            std::shared_future<std::shared_ptr<util::Point2D>> launch_async_loop(
                    const std::vector<util::Point2D>& positions,
                    const std::vector<Hint>& hints) ;

            void cancel() noexcept ;

            const std::vector<unsigned char>& matched_counts() const noexcept ;

            const std::size_t& drawable_hints_num() const noexcept ;
        } ;
    }
}

#endif
