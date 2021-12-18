#include <doctest.h>

#include "core/mode.hpp"
#include "core/rc_parser.cpp"

#include <string>

using namespace vind::core ;

// remove_dbquote_comment
TEST_CASE("(rcparser::remove_dbquote_comment) Normal input without strings") {
    std::string str("foo bar\" comment area") ;
    CHECK_NOTHROW(remove_dbquote_comment(str)) ;
    CHECK_EQ(str, "foo bar") ;
}
TEST_CASE("(rcparser::remove_dbquote_comment) all comment") {
    std::string str("  \" comment area \"aa\" bb") ;
    CHECK_NOTHROW(remove_dbquote_comment(str)) ;
    CHECK(str.empty()) ;
}

TEST_CASE("(rcparser::remove_dbquote_comment) Normal input with strings") {
    std::string str("foo bar \"str1\" \"str2\"\" comment area") ;
    CHECK_NOTHROW(remove_dbquote_comment(str)) ;
    CHECK_EQ(str, "foo bar \"str1\" \"str2\"") ;
}


// divide_cmd_and_args
TEST_CASE("(rcparser::divide_cmd_and_args) Empty line") {
    auto [cmd, args] = divide_cmd_and_args("") ;

    CHECK(cmd.empty()) ;
    CHECK(args.empty()) ;
}

TEST_CASE("(rcparser::divide_cmd_and_args) line includes only spaces") {
    auto [cmd, args] = divide_cmd_and_args("    ") ;

    CHECK(cmd.empty()) ;
    CHECK(args.empty()) ;
}

TEST_CASE("(rcparser::divide_cmd_and_args) no argument without delimiter") {
    auto [cmd, args] = divide_cmd_and_args("set") ;
    CHECK_EQ(cmd, "set") ;
    CHECK(args.empty()) ;
}

TEST_CASE("(rcparser::divide_cmd_and_args) no argument with delimiter") {
    auto [cmd, args] = divide_cmd_and_args("set ") ;
    CHECK_EQ(cmd, "set") ;
    CHECK(args.empty()) ;
}

TEST_CASE("(rcparser::divide_cmd_and_args) Normal input") {
    auto [cmd, args] = divide_cmd_and_args("set foo  ") ;
    CHECK_EQ(cmd, "set") ;
    CHECK_EQ(args, "foo") ;
}


// extract_single_arg
TEST_CASE("(rcparser::extract_single_arg) Empty argument") {
    CHECK_EQ(extract_single_arg(""), "") ;
}

TEST_CASE("(rcparser::extract_single_arg) The arguments include only spaces") {
    CHECK_EQ(extract_single_arg("     "), "") ;
}

TEST_CASE("(rcparser::extract_single_arg) Give multiple arguments") {
    CHECK_THROWS(extract_single_arg("foo bar")) ;
}

TEST_CASE("(rcparser::extract_single_arg) Normal input") {
    CHECK_EQ(extract_single_arg("  foo   "), "foo") ;
}


// extract_double_args
TEST_CASE("(rcparser::extract_double_args) Empty input") {
    auto [arg1, arg2] = extract_double_args("") ;
    CHECK(arg1.empty()) ;
    CHECK(arg2.empty()) ;
}

TEST_CASE("(rcparser::extract_double_args) The argments include only spaces") {
    auto [arg1, arg2] = extract_double_args("     ") ;
    CHECK(arg1.empty()) ;
    CHECK(arg2.empty()) ;
}

TEST_CASE("(rcparser::extract_double_args) It includes only one argument without delimiter") {
    auto [arg1, arg2] = extract_double_args("foo") ;
    CHECK_EQ(arg1, "foo") ;
    CHECK(arg2.empty()) ;
}

TEST_CASE("(rcparser::extract_double_args) It includes only one argument with delimiter") {
    auto [arg1, arg2] = extract_double_args("foo ") ;
    CHECK_EQ(arg1, "foo") ;
    CHECK(arg2.empty()) ;
}

TEST_CASE("(rcparser::extract_double_args) Normal input") {
    auto [arg1, arg2] = extract_double_args("foo bar  ") ;
    CHECK_EQ(arg1, "foo") ;
    CHECK_EQ(arg2, "bar") ;
}

TEST_CASE("(rcparser::extract_double_args) Triple argument") {
    CHECK_THROWS(extract_double_args("foo bar bar")) ;
}


// divide_key_and_value
TEST_CASE("(rcparser::divide_key_and_value) Empty input") {
    auto [key, val] = divide_key_and_value("") ;
    CHECK(key.empty()) ;
    CHECK(val.empty()) ;
}

TEST_CASE("(rcparser::divide_key_and_value) Empty delimiter") {
    auto [key, val] = divide_key_and_value("foo", "") ;
    CHECK_EQ(key, "foo") ;
    CHECK(val.empty()) ;
}

TEST_CASE("(rcparser::divide_key_and_value) The arguments has no delimiter.") {
    auto [key, val] = divide_key_and_value("foo bar", "=") ;
    CHECK_EQ(key, "foo bar") ;
    CHECK(val.empty()) ;
}

TEST_CASE("(rcparser::divide_key_and_value) Empty values") {
    auto [key, val] = divide_key_and_value("foo =  ") ;
    CHECK_EQ(key, "foo") ;
    CHECK(val.empty()) ;
}

TEST_CASE("(rcparser::divide_key_and_value) Normal inputs") {
    auto [key, val] = divide_key_and_value(" foo  = bar ") ;
    CHECK_EQ(key, "foo") ;
    CHECK_EQ(val, "bar") ;
}

// divide_prefix_and_cmd
TEST_CASE("(rcparser::divide_prefix_and_cmd) The string has no keyword") {
    auto [prefix, cmd] = divide_prefix_and_cmd("abcde", "n") ;
    CHECK(prefix.empty()) ;
    CHECK_EQ(cmd, "abcde") ;
}

TEST_CASE("(rcparser::divide_prefix_and_cmd)) Normal input") {
    auto [prefix, cmd] = divide_prefix_and_cmd("inoremap", "n") ;
    CHECK_EQ(prefix, "i") ;
    CHECK_EQ(cmd, "noremap") ;
}
