/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

// -----------------------------------------------------------------------------

#include <string>
#include <regex>

#include "maddy/lineparser.h"

// -----------------------------------------------------------------------------

namespace maddy {

// -----------------------------------------------------------------------------

/**
 * BreakLineParser
 *
 * @class
 */
class BreakLineParser : public LineParser
{
public:
  /**
   * Parse
   *
   * From Markdown: `text\r\n text`
   *
   * To HTML: `text<br> text`
   *
   * @method
   * @param {std::string&} line The line to interpret
   * @return {void}
   */
  void
  Parse(std::string& line) override
  {
    static std::regex re(R"((\r\n|\r))");
    static std::string replacement = "<br>";

    line = std::regex_replace(line, re, replacement);
  }
}; // class BreakLineParser

// -----------------------------------------------------------------------------

} // namespace maddy
