#include <doctest.h>

#include "util/container.hpp"

using namespace vind::util ;

// util::remove_deplication
TEST_CASE("(util::remove_deplication) a input is empty") {
    std::vector<int> v{} ;
    std::vector<int> expect{} ;
    remove_deplication(v) ;
    CHECK_EQ(v, expect) ;
}

TEST_CASE("(util::remove_deplication) normal") {
    std::vector<int> v{1, 2, 2, 3, 4, 5, 5, 1} ;
    std::vector<int> expect{1, 2, 3, 4, 5} ;
    remove_deplication(v) ;
    CHECK_EQ(v, expect) ;
}


// util::sizeof_array
TEST_CASE("(util::sizeof_array) normal") {
    int ar[10] ;
    CHECK_EQ(sizeof_array(ar), 10) ;
}


// util::remove_from_top
TEST_CASE("(util::remove_from_top) a input is empty") {
    std::vector<int> v{} ;
    std::vector<int> expect{} ;
    remove_from_top(v, 1) ;
    CHECK_EQ(v, expect) ;
}

TEST_CASE("(util::remove_from_top) the removing size is over") {
    std::vector<int> v{1, 2, 3, 4} ;
    std::vector<int> expect{} ;
    remove_from_top(v, 10) ;
    CHECK_EQ(v, expect) ;
}

TEST_CASE("(util::remove_from_top) normal") {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7} ;
    std::vector<int> expect{4, 5, 6, 7} ;
    remove_from_top(v, 3) ;
    CHECK_EQ(v, expect) ;
}


// util::remove_from_back
TEST_CASE("(util::remove_from_back) a input is empty") {
    std::vector<int> v{} ;
    std::vector<int> expect{} ;
    remove_from_back(v, 1) ;
    CHECK_EQ(v, expect) ;
}

TEST_CASE("(util::remove_from_back) a removing size is over") {
    std::vector<int> v{1, 2, 3, 4} ;
    std::vector<int> expect{} ;
    remove_from_back(v, 10) ;
    CHECK_EQ(v, expect) ;
}

TEST_CASE("(util::remove_from_back) normal") {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7} ;
    std::vector<int> expect{1, 2, 3, 4} ;
    remove_from_back(v, 3) ;
    CHECK_EQ(v, expect) ;
}
