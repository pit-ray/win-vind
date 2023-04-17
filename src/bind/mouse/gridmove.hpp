#ifndef _GRIDMOVE_HPP
#define _GRIDMOVE_HPP

#include "bind/bindedfunc.hpp"

#include <memory>


namespace vind
{
    namespace bind
    {
        class GridMove : public BindedFuncVoid<GridMove> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit GridMove() ;
            virtual ~GridMove() noexcept ;

            void sprocess(
                std::uint16_t count, const std::string& args) ;

            GridMove(GridMove&&) ;
            GridMove& operator=(GridMove&&) ;
            GridMove(const GridMove&)            = delete ;
            GridMove& operator=(const GridMove&) = delete ;

            void reconstruct() override ;
        } ;
    }
}

#endif
