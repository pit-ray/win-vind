/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

// -----------------------------------------------------------------------------

namespace maddy {

// -----------------------------------------------------------------------------

/**
 * ParserConfig
 *
 * @class
 */
struct ParserConfig
{
  bool isEmphasizedParserEnabled;
  bool isHTMLWrappedInParagraph;

  ParserConfig()
    : isEmphasizedParserEnabled(true)
    , isHTMLWrappedInParagraph(true)
  {}
}; // class ParserConfig

// -----------------------------------------------------------------------------

} // namespace maddy
