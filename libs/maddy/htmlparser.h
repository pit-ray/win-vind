/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

// -----------------------------------------------------------------------------

#include <functional>
#include <string>

#include "maddy/blockparser.h"

// -----------------------------------------------------------------------------

namespace maddy {

// -----------------------------------------------------------------------------

/**
 * HtmlParser
 *
 * @class
 */
class HtmlParser : public BlockParser
{
public:
  /**
   * ctor
   *
   * @method
   * @param {std::function<void(std::string&)>} parseLineCallback
   * @param {std::function<std::shared_ptr<BlockParser>(const std::string& line)>} getBlockParserForLineCallback
   */
   HtmlParser(
    std::function<void(std::string&)> parseLineCallback,
    std::function<std::shared_ptr<BlockParser>(const std::string& line)> getBlockParserForLineCallback
  )
    : BlockParser(parseLineCallback, getBlockParserForLineCallback)
    , isStarted(false)
    , isFinished(false)
    , isGreaterThanFound(false)
  {}

  /**
   * IsStartingLine
   *
   * If the line is starting with `<`, HTML is expected to follow.
   * Nothing after that will be parsed, it only is copied.
   *
   * @method
   * @param {const std::string&} line
   * @return {bool}
   */
  static bool
  IsStartingLine(const std::string& line)
  {
    return line[0] == '<';
  }

  /**
   * IsFinished
   *
   * `>` followed by an empty line will end the HTML block.
   *
   * @method
   * @return {bool}
   */
  bool
  IsFinished() const override
  {
    return this->isFinished;
  }

protected:
  bool
  isInlineBlockAllowed() const override
  {
    return false;
  }

  bool
  isLineParserAllowed() const override
  {
    return false;
  }

  void
  parseBlock(std::string& line) override
  {
    if (!this->isStarted)
    {
      this->isStarted = true;
    }

    if (!line.empty() && line[line.size() - 1] == '>')
    {
      this->isGreaterThanFound = true;
      return;
    }

    if (line.empty() && this->isGreaterThanFound)
    {
      this->isFinished = true;
      return;
    }

    if (!line.empty() && this->isGreaterThanFound)
    {
      this->isGreaterThanFound = false;
    }

    if (!line.empty())
    {
      line += " ";
    }
  }

private:
  bool isStarted;
  bool isFinished;
  bool isGreaterThanFound;
}; // class HtmlParser

// -----------------------------------------------------------------------------

} // namespace maddy
