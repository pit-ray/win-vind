/*
 * This project is licensed under the MIT license. For more information see the
 * LICENSE file.
 */
#pragma once

// -----------------------------------------------------------------------------

#include <memory>
#include <functional>
#include <string>

#include "maddy/parserconfig.h"

// BlockParser
#include "maddy/checklistparser.h"
#include "maddy/codeblockparser.h"
#include "maddy/headlineparser.h"
#include "maddy/horizontallineparser.h"
#include "maddy/htmlparser.h"
#include "maddy/orderedlistparser.h"
#include "maddy/paragraphparser.h"
#include "maddy/quoteparser.h"
#include "maddy/tableparser.h"
#include "maddy/unorderedlistparser.h"

// LineParser
#include "maddy/breaklineparser.h"
#include "maddy/emphasizedparser.h"
#include "maddy/imageparser.h"
#include "maddy/inlinecodeparser.h"
#include "maddy/italicparser.h"
#include "maddy/linkparser.h"
#include "maddy/strikethroughparser.h"
#include "maddy/strongparser.h"

// -----------------------------------------------------------------------------

namespace maddy {

// -----------------------------------------------------------------------------

/**
 * Parser
 *
 * Transforms Markdown to HTML
 *
 * @class
 */
class Parser
{
public:
  /**
   * ctor
   *
   * Initializes all `LineParser`
   *
   * @method
   */
  Parser(std::shared_ptr<ParserConfig> config = nullptr)
    : config(config)
    , breakLineParser(std::make_shared<BreakLineParser>())
    , emphasizedParser(std::make_shared<EmphasizedParser>())
    , imageParser(std::make_shared<ImageParser>())
    , inlineCodeParser(std::make_shared<InlineCodeParser>())
    , italicParser(std::make_shared<ItalicParser>())
    , linkParser(std::make_shared<LinkParser>())
    , strikeThroughParser(std::make_shared<StrikeThroughParser>())
    , strongParser(std::make_shared<StrongParser>())
  {}

  /**
   * Parse
   *
   * @method
   * @param {const std::istream&} markdown
   * @return {std::string} HTML
   */
  std::string
  Parse(std::istream& markdown) const
  {
    std::string result = "";
    std::shared_ptr<BlockParser> currentBlockParser = nullptr;

    for (std::string line; std::getline(markdown, line);)
    {
      if (!currentBlockParser)
      {
        currentBlockParser = getBlockParserForLine(line);
      }

      if (currentBlockParser)
      {
        currentBlockParser->AddLine(line);

        if (currentBlockParser->IsFinished())
        {
          result += currentBlockParser->GetResult().str();
          currentBlockParser = nullptr;
        }
      }
    }

    // make sure, that all parsers are finished
    if (currentBlockParser)
    {
      std::string emptyLine = "";
      currentBlockParser->AddLine(emptyLine);
      if (currentBlockParser->IsFinished())
      {
        result += currentBlockParser->GetResult().str();
        currentBlockParser = nullptr;
      }
    }

    return result;
  }

private:
  std::shared_ptr<ParserConfig> config;
  std::shared_ptr<BreakLineParser> breakLineParser;
  std::shared_ptr<EmphasizedParser> emphasizedParser;
  std::shared_ptr<ImageParser> imageParser;
  std::shared_ptr<InlineCodeParser> inlineCodeParser;
  std::shared_ptr<ItalicParser> italicParser;
  std::shared_ptr<LinkParser> linkParser;
  std::shared_ptr<StrikeThroughParser> strikeThroughParser;
  std::shared_ptr<StrongParser> strongParser;

  // block parser have to run before
  void
  runLineParser(std::string& line) const
  {
    // Attention! ImageParser has to be before LinkParser
    this->imageParser->Parse(line);
    this->linkParser->Parse(line);

    // Attention! StrongParser has to be before EmphasizedParser
    this->strongParser->Parse(line);

    if (!this->config || this->config->isEmphasizedParserEnabled)
    {
      this->emphasizedParser->Parse(line);
    }

    this->strikeThroughParser->Parse(line);

    this->inlineCodeParser->Parse(line);

    this->italicParser->Parse(line);

    this->breakLineParser->Parse(line);
  }

  std::shared_ptr<BlockParser>
  getBlockParserForLine(const std::string& line) const
  {
    std::shared_ptr<BlockParser> parser;

    if (maddy::CodeBlockParser::IsStartingLine(line))
    {
      parser = std::make_shared<maddy::CodeBlockParser>(
        nullptr,
        nullptr
      );
    }
    else if (maddy::HeadlineParser::IsStartingLine(line))
    {
      parser = std::make_shared<maddy::HeadlineParser>(
        nullptr,
        nullptr
      );
    }
    else if (maddy::HorizontalLineParser::IsStartingLine(line))
    {
      parser = std::make_shared<maddy::HorizontalLineParser>(
        nullptr,
        nullptr
      );
    }
    else if (maddy::QuoteParser::IsStartingLine(line))
    {
      parser = std::make_shared<maddy::QuoteParser>(
        [this](std::string& line){ this->runLineParser(line); },
        [this](const std::string& line){ return this->getBlockParserForLine(line); }
      );
    }
    else if (maddy::TableParser::IsStartingLine(line))
    {
      parser = std::make_shared<maddy::TableParser>(
        [this](std::string& line){ this->runLineParser(line); },
        nullptr
      );
    }
    else if (maddy::ChecklistParser::IsStartingLine(line))
    {
      parser = this->createChecklistParser();
    }
    else if (maddy::OrderedListParser::IsStartingLine(line))
    {
      parser = this->createOrderedListParser();
    }
    else if (maddy::UnorderedListParser::IsStartingLine(line))
    {
      parser = this->createUnorderedListParser();
    }
    else if (
      this->config &&
      !this->config->isHTMLWrappedInParagraph &&
      maddy::HtmlParser::IsStartingLine(line)
    )
    {
      parser = std::make_shared<maddy::HtmlParser>(nullptr, nullptr);
    }
    else if (maddy::ParagraphParser::IsStartingLine(line))
    {
      parser = std::make_shared<maddy::ParagraphParser>(
        [this](std::string& line){ this->runLineParser(line); },
        nullptr
      );
    }

    return parser;
  }

  std::shared_ptr<BlockParser>
  createChecklistParser() const
  {
    return std::make_shared<maddy::ChecklistParser>(
      [this](std::string& line){ this->runLineParser(line); },
      [this](const std::string& line)
      {
        std::shared_ptr<BlockParser> parser;

        if (maddy::ChecklistParser::IsStartingLine(line))
        {
          parser = this->createChecklistParser();
        }

        return parser;
      }
    );
  }

  std::shared_ptr<BlockParser>
  createOrderedListParser() const
  {
    return std::make_shared<maddy::OrderedListParser>(
      [this](std::string& line){ this->runLineParser(line); },
      [this](const std::string& line)
      {
        std::shared_ptr<BlockParser> parser;

        if (maddy::OrderedListParser::IsStartingLine(line))
        {
          parser = this->createOrderedListParser();
        }
        else if (maddy::UnorderedListParser::IsStartingLine(line))
        {
          parser = this->createUnorderedListParser();
        }

        return parser;
      }
    );
  }

  std::shared_ptr<BlockParser>
  createUnorderedListParser() const
  {
    return std::make_shared<maddy::UnorderedListParser>(
      [this](std::string& line){ this->runLineParser(line); },
      [this](const std::string& line)
      {
        std::shared_ptr<BlockParser> parser;

        if (maddy::OrderedListParser::IsStartingLine(line))
        {
          parser = this->createOrderedListParser();
        }
        else if (maddy::UnorderedListParser::IsStartingLine(line))
        {
          parser = this->createUnorderedListParser();
        }

        return parser;
      }
    );
  }
}; // class Parser

// -----------------------------------------------------------------------------

} // namespace maddy
