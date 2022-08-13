#include <doctest/doctest.h>

#include "core/mapsolver.hpp"


namespace vind
{
    namespace bind
    {
        SystemCall BindedFunc::process() {
            return SystemCall::NOTHING ;
        }

        BindedFunc::SPtr ref_global_func_byname(const std::string&) {
            return nullptr ;
        }

        BindedFunc::SPtr ref_global_funcs_bynames(const std::string&) {
            return nullptr ;
        }

        BindedFunc::SPtr ref_global_funcs_bynames(std::string&&) {
            return nullptr ;
        }
    }
}


TEST_SUITE("core/mapsolver") {
    using namespace vind::core ;

    TEST_CASE("MapSolver::*_default") {
        MapSolver solver(true) ;
        solver.add_default("h", "{<left>}") ;
        solver.add_default("j", "{<down>}") ;
        solver.add_default("k", "{<up>}") ;
        solver.add_default("l", "{<right>}") ;
        solver.deploy_default() ;
        solver.deploy() ;

        SUBCASE("case1: ") {
            auto triggers = solver.get_trigger_commands() ;
            auto targets = solver.get_target_commands() ;

            CHECK_EQ(triggers.size(), 4) ;

            CHECK_EQ(targets.size(), 4) ;
        }
    }
}
