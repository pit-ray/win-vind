#ifndef _BACKGROUND_HPP
#define _BACKGROUND_HPP

#include "opt/option.hpp"

#include <memory>
#include <vector>


namespace vind
{
    namespace core
    {
        class Background {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit Background() ;
            explicit Background(const std::vector<opt::Option::SPtr>& opts) ;
            explicit Background(std::vector<opt::Option::SPtr>&& opts) ;

            virtual ~Background() noexcept ;

            Background(Background&&) ;
            Background& operator=(Background&&) ;

            // If you make some loop functions, the function is needed to call.
            // It includes Sleep().
            void update() ;

            Background(const Background&) = delete ;
            Background& operator=(const Background&) = delete ;
        } ;
    }
}

#endif
