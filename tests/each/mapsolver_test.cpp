#include <doctest/doctest.h>

#include "core/mapsolver.hpp"


namespace vind
{
    namespace bind
    {
        SystemCall BindedFunc::process(std::uint16_t, const std::string&) {
            return SystemCall::SUCCEEDED ;
        }

        BindedFunc::SPtr ref_global_func_byname(const std::string& name) {
            if(name == "click_right") {
                return std::make_unique<BindedFunc>(name) ;
            }
            return nullptr ;
        }

        BindedFunc::SPtr ref_global_funcs_bynames(const std::string&) {
            return nullptr ;
        }
    }
}


TEST_SUITE("core/mapsolver") {
    using namespace vind::core ;

    TEST_CASE("default") {
        MapSolver solver{} ;
        solver.add_default("h", "{<left>}") ;
        solver.add_default("j", "{<down>}") ;
        solver.add_default("k", "{<up>}") ;
        solver.add_default("l", "{<right>}") ;
        solver.deploy_default(false) ;

        auto triggers = solver.get_trigger_commands() ;
        CHECK_EQ(triggers.size(), 0) ;

        auto targets = solver.get_target_commands() ;
        CHECK_EQ(targets.size(), 0) ;

        solver.deploy() ;

        SUBCASE("case1: add_default") {
            auto triggers = solver.get_trigger_commands() ;
            CHECK_EQ(triggers.size(), 4) ;

            std::vector<std::vector<CmdUnitSet>> expect_triggers {
                {{KEYCODE_H}},
                {{KEYCODE_J}},
                {{KEYCODE_K}},
                {{KEYCODE_L}}
            } ;

            for(int i = 0 ; i < triggers.size() ; i ++) {
                for(int j = 0 ; j < triggers[i].size() ; j ++) {
                    CHECK_EQ(*triggers[i][j], expect_triggers[i][j]) ;
                }
            }

            auto targets = solver.get_target_commands() ;
            CHECK_EQ(targets.size(), 4) ;

            std::vector<std::vector<CmdUnitSet>> expect_targets {
                {{KEYCODE_LEFT}},
                {{KEYCODE_DOWN}},
                {{KEYCODE_UP}},
                {{KEYCODE_RIGHT}}
            } ;

            for(int i = 0 ; i < targets.size() ; i ++) {
                for(int j = 0 ; j < targets[i].size() ; j ++) {
                    CHECK_EQ(*targets[i][j], expect_targets[i][j]) ;
                }
            }
        }

        SUBCASE("case2: remove_default") {
            solver.remove_default("j") ;
            solver.remove_default("k") ;
            solver.deploy_default() ;

            auto triggers = solver.get_trigger_commands() ;
            CHECK_EQ(triggers.size(), 4) ;

            auto targets = solver.get_target_commands() ;
            CHECK_EQ(targets.size(), 4) ;

            solver.deploy() ;

            triggers = solver.get_trigger_commands() ;
            CHECK_EQ(triggers.size(), 2) ;

            std::vector<std::vector<CmdUnitSet>> expect_triggers {
                {{KEYCODE_H}},
                {{KEYCODE_L}}
            } ;

            for(int i = 0 ; i < triggers.size() ; i ++) {
                for(int j = 0 ; j < triggers[i].size() ; j ++) {
                    CHECK_EQ(*triggers[i][j], expect_triggers[i][j]) ;
                }
            }

            targets = solver.get_target_commands() ;
            CHECK_EQ(targets.size(), 2) ;

            std::vector<std::vector<CmdUnitSet>> expect_targets {
                {{KEYCODE_LEFT}},
                {{KEYCODE_RIGHT}}
            } ;

            for(int i = 0 ; i < targets.size() ; i ++) {
                for(int j = 0 ; j < targets[i].size() ; j ++) {
                    CHECK_EQ(*targets[i][j], expect_targets[i][j]) ;
                }
            }
        }

        SUBCASE("case3: clear_default") {
            solver.clear_default() ;
            solver.deploy_default() ;

            auto triggers = solver.get_trigger_commands() ;
            CHECK_EQ(triggers.size(), 4) ;

            auto targets = solver.get_target_commands() ;
            CHECK_EQ(targets.size(), 4) ;

            solver.deploy() ;

            triggers = solver.get_trigger_commands() ;
            CHECK_EQ(triggers.size(), 0) ;

            targets = solver.get_target_commands() ;
            CHECK_EQ(targets.size(), 0) ;
        }

        SUBCASE("case4: deploy_default(true)") {
            solver.add_default("a", "h") ;
            solver.add_default("h", "<c-u>") ;
            solver.add_default("<s-f>", "k") ;
            solver.add_default("cc", "<s-f>") ;
            solver.add_default("d<c-g>", "cc") ;
            solver.deploy_default(true) ;
            solver.deploy() ;

            auto triggers = solver.get_trigger_commands() ;
            CHECK_EQ(triggers.size(), 8) ;

            std::vector<std::vector<CmdUnitSet>> expect_triggers {
                {{KEYCODE_J}},
                {{KEYCODE_K}},
                {{KEYCODE_L}},

                {{KEYCODE_A}},
                {{KEYCODE_H}},
                {{KEYCODE_SHIFT, KEYCODE_F}},
                {{KEYCODE_C}, {KEYCODE_C}},
                {{KEYCODE_D}, {KEYCODE_CTRL, KEYCODE_G}},
            } ;

            for(int i = 0 ; i < triggers.size() ; i ++) {
                for(int j = 0 ; j < triggers[i].size() ; j ++) {
                    CHECK_EQ(*triggers[i][j], expect_triggers[i][j]) ;
                }
            }

            auto targets = solver.get_target_commands() ;
            CHECK_EQ(targets.size(), 8) ;

            std::vector<std::vector<CmdUnitSet>> expect_targets {
                {{KEYCODE_DOWN}},
                {{KEYCODE_UP}},
                {{KEYCODE_RIGHT}},

                {{KEYCODE_CTRL, KEYCODE_U}},
                {{KEYCODE_CTRL, KEYCODE_U}},
                {{KEYCODE_UP}},
                {{KEYCODE_UP}},
                {{KEYCODE_UP}},
            } ;

            for(int i = 0 ; i < targets.size() ; i ++) {
                for(int j = 0 ; j < targets[i].size() ; j ++) {
                    CHECK_EQ(*targets[i][j], expect_targets[i][j]) ;
                }
            }
        }

        SUBCASE("case5: self-mapping") {
            solver.add_default("<left>", "h") ;
            solver.add_default("b", "b") ;
            solver.deploy_default() ;
            solver.deploy() ;

            auto triggers = solver.get_trigger_commands() ;
            CHECK_EQ(triggers.size(), 3) ;

            std::vector<std::vector<CmdUnitSet>> expect_triggers {
                {{KEYCODE_J}},
                {{KEYCODE_K}},
                {{KEYCODE_L}}
            } ;

            for(int i = 0 ; i < triggers.size() ; i ++) {
                for(int j = 0 ; j < triggers[i].size() ; j ++) {
                    CHECK_EQ(*triggers[i][j], expect_triggers[i][j]) ;
                }
            }

            auto targets = solver.get_target_commands() ;
            CHECK_EQ(targets.size(), 3) ;

            std::vector<std::vector<CmdUnitSet>> expect_targets {
                {{KEYCODE_DOWN}},
                {{KEYCODE_UP}},
                {{KEYCODE_RIGHT}}
            } ;

            for(int i = 0 ; i < targets.size() ; i ++) {
                for(int j = 0 ; j < targets[i].size() ; j ++) {
                    CHECK_EQ(*targets[i][j], expect_targets[i][j]) ;
                }
            }
        }
    }

    TEST_CASE("noremap") {
        MapSolver solver{} ;

        solver.add_default("h", "{<left>}") ;
        solver.add_default("j", "{<down>}") ;
        solver.add_default("k", "{<up>}") ;
        solver.add_default("l", "{<right>}") ;

        solver.deploy_default() ;

        solver.add_noremap("<f1>", "h") ;
        solver.add_noremap("b", "{p}kl") ;
        solver.add_noremap("c", "lh{bu}") ;

        solver.deploy() ;

        SUBCASE("case1: deploy") {
            auto triggers = solver.get_trigger_commands() ;
            CHECK_EQ(triggers.size(), 7) ;

            std::vector<std::vector<CmdUnitSet>> expect_triggers {
                {{KEYCODE_H}},
                {{KEYCODE_J}},
                {{KEYCODE_K}},
                {{KEYCODE_L}},

                {{KEYCODE_F1}},
                {{KEYCODE_B}},
                {{KEYCODE_C}}
            } ;

            for(int i = 0 ; i < triggers.size() ; i ++) {
                for(int j = 0 ; j < triggers[i].size() ; j ++) {
                    CHECK_EQ(*triggers[i][j], expect_triggers[i][j]) ;
                }
            }

            auto targets = solver.get_target_commands() ;
            CHECK_EQ(targets.size(), 7) ;

            std::vector<std::vector<CmdUnitSet>> expect_targets {
                {{KEYCODE_LEFT}},
                {{KEYCODE_DOWN}},
                {{KEYCODE_UP}},
                {{KEYCODE_RIGHT}},

                {{KEYCODE_LEFT}},
                {{KEYCODE_P}, {KEYCODE_UP}, {KEYCODE_RIGHT}},
                {{KEYCODE_RIGHT}, {KEYCODE_LEFT}, {KEYCODE_B}, {KEYCODE_U}}
            } ;

            for(int i = 0 ; i < targets.size() ; i ++) {
                for(int j = 0 ; j < targets[i].size() ; j ++) {
                    CHECK_EQ(*targets[i][j], expect_targets[i][j]) ;
                }
            }
        }
    }

    TEST_CASE("map") {
        MapSolver solver{} ;

        solver.add_default("h", "{<left>}") ;
        solver.add_default("j", "{<down>}") ;
        solver.add_default("k", "{<up>}") ;
        solver.add_default("l", "{<right>}") ;

        solver.deploy_default() ;

        solver.add_noremap("b", "{aaaa}") ;
        solver.add_map("b", "jkh") ;
        solver.add_map("c<s-j>", "bjk") ;
        solver.add_map("gg", "kk") ;
        solver.add_map("x", "ggggg") ;

        solver.deploy() ;

        SUBCASE("case1: deploy") {
            auto triggers = solver.get_trigger_commands() ;
            CHECK_EQ(triggers.size(), 8) ;

            std::vector<std::vector<CmdUnitSet>> expect_triggers {
                {{KEYCODE_H}},
                {{KEYCODE_J}},
                {{KEYCODE_K}},
                {{KEYCODE_L}},

                {{KEYCODE_B}},
                {{KEYCODE_C}, {KEYCODE_SHIFT, KEYCODE_J}},
                {{KEYCODE_G}, {KEYCODE_G}},
                {{KEYCODE_X}},
            } ;

            for(int i = 0 ; i < triggers.size() ; i ++) {
                for(int j = 0 ; j < triggers[i].size() ; j ++) {
                    CHECK_EQ(*triggers[i][j], expect_triggers[i][j]) ;
                }
            }

            auto targets = solver.get_target_commands() ;
            CHECK_EQ(targets.size(), 8) ;

            std::vector<std::vector<CmdUnitSet>> expect_targets {
                {{KEYCODE_LEFT}},
                {{KEYCODE_DOWN}},
                {{KEYCODE_UP}},
                {{KEYCODE_RIGHT}},

                {{KEYCODE_DOWN}, {KEYCODE_UP}, {KEYCODE_LEFT}},
                {{KEYCODE_DOWN}, {KEYCODE_UP}, {KEYCODE_LEFT}, {KEYCODE_DOWN}, {KEYCODE_UP}},
                {{KEYCODE_UP}, {KEYCODE_UP}},
                {{KEYCODE_UP}, {KEYCODE_UP}, {KEYCODE_UP}, {KEYCODE_UP}, {KEYCODE_G}}
            } ;

            for(int i = 0 ; i < targets.size() ; i ++) {
                for(int j = 0 ; j < targets[i].size() ; j ++) {
                    CHECK_EQ(*targets[i][j], expect_targets[i][j]) ;
                }
            }
        }

        SUBCASE("case2: remove") {
            solver.remove("b") ;
            solver.remove("gg") ;

            solver.deploy() ;

            auto triggers = solver.get_trigger_commands() ;
            CHECK_EQ(triggers.size(), 6) ;

            std::vector<std::vector<CmdUnitSet>> expect_triggers {
                {{KEYCODE_H}},
                {{KEYCODE_J}},
                {{KEYCODE_K}},
                {{KEYCODE_L}},

                {{KEYCODE_C}, {KEYCODE_SHIFT, KEYCODE_J}},
                {{KEYCODE_X}},
            } ;

            for(int i = 0 ; i < triggers.size() ; i ++) {
                for(int j = 0 ; j < triggers[i].size() ; j ++) {
                    CHECK_EQ(*triggers[i][j], expect_triggers[i][j]) ;
                }
            }

            auto targets = solver.get_target_commands() ;
            CHECK_EQ(targets.size(), 6) ;

            std::vector<std::vector<CmdUnitSet>> expect_targets {
                {{KEYCODE_LEFT}},
                {{KEYCODE_DOWN}},
                {{KEYCODE_UP}},
                {{KEYCODE_RIGHT}},

                {{KEYCODE_B}, {KEYCODE_DOWN}, {KEYCODE_UP}},
                {{KEYCODE_G}, {KEYCODE_G}, {KEYCODE_G}, {KEYCODE_G}, {KEYCODE_G}}
            } ;

            for(int i = 0 ; i < targets.size() ; i ++) {
                for(int j = 0 ; j < targets[i].size() ; j ++) {
                    CHECK_EQ(*targets[i][j], expect_targets[i][j]) ;
                }
            }
        }

        SUBCASE("case3: clear") {
            solver.clear() ;
            solver.deploy() ;

            auto triggers = solver.get_trigger_commands() ;
            CHECK_EQ(triggers.size(), 4) ;

            std::vector<std::vector<CmdUnitSet>> expect_triggers {
                {{KEYCODE_H}},
                {{KEYCODE_J}},
                {{KEYCODE_K}},
                {{KEYCODE_L}}
            } ;

            for(int i = 0 ; i < triggers.size() ; i ++) {
                for(int j = 0 ; j < triggers[i].size() ; j ++) {
                    CHECK_EQ(*triggers[i][j], expect_triggers[i][j]) ;
                }
            }

            auto targets = solver.get_target_commands() ;
            CHECK_EQ(targets.size(), 4) ;

            std::vector<std::vector<CmdUnitSet>> expect_targets {
                {{KEYCODE_LEFT}},
                {{KEYCODE_DOWN}},
                {{KEYCODE_UP}},
                {{KEYCODE_RIGHT}}
            } ;

            for(int i = 0 ; i < targets.size() ; i ++) {
                for(int j = 0 ; j < targets[i].size() ; j ++) {
                    CHECK_EQ(*targets[i][j], expect_targets[i][j]) ;
                }
            }
        }
    }

    TEST_CASE("map") {
        MapSolver solver{} ;
        solver.add_default("a", "<click_right>") ;
        solver.deploy_default() ;

        solver.add_map("a", "{g}") ;
        solver.deploy() ;

        auto triggers = solver.get_trigger_commands() ;
        CHECK_EQ(triggers.size(), 1) ;

        CHECK_EQ(*triggers[0][0], CmdUnitSet{KEYCODE_A}) ;

        auto targets = solver.get_target_commands() ;
        CHECK_EQ(targets.size(), 1) ;

        CHECK_EQ(*targets[0][0], CmdUnitSet{KEYCODE_G}) ;
    }

    TEST_CASE("map_command_to") {


    }
}
