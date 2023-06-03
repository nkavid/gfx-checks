// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/basename_declaration_check.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

#include <filesystem>
#include <iostream>

#include <clang-tidy/utils/OptionsUtils.h>

using namespace clang::ast_matchers;

namespace clang::tidy::gfx
{
namespace
{
using utils::options::parseStringList;

std::optional<naming::Case> getCase(const std::vector<StringRef>& parsedStringList)
{
  StringRef caseOption{"aNy_CasE"};

  if (parsedStringList.size() == 1)
  {
    caseOption = parsedStringList[0];
  }

  if (caseOption == "lower_case")
  {
    return naming::Case::snake_case;
  }
  if (caseOption == "CamelCase")
  {
    return naming::Case::PascalCase;
  }
  if (caseOption == "camelBack")
  {
    return naming::Case::camelCase;
  }

  return std::nullopt;
}

void convertString(std::string& input, naming::Case namingCase)
{
  constexpr auto toLowerLambda = [](unsigned char character) {
    return std::tolower(character);
  };

  if (namingCase == naming::Case::snake_case)
  {
    input.erase(std::remove(input.begin(), input.end(), '_'), input.end());
  }

  std::transform(input.begin(), input.end(), input.begin(), toLowerLambda);
}

bool findAnyMatch(const std::filesystem::path& filename,
                  const std::vector<const NamedDecl*>& namedDecls,
                  const naming::Map& namingMap)
{
  bool foundAnyMatch{false};
  for (const auto* namedDecl : namedDecls)
  {
    auto basename = filename.stem().filename().string();
    convertString(basename, namingMap.at(naming::Declaration::File));

    if (const auto* namespaceDecl = dyn_cast<NamespaceDecl>(namedDecl))
    {
      auto nsStr = namespaceDecl->getNameAsString();
      convertString(nsStr, namingMap.at(naming::Declaration::Namespace));
      foundAnyMatch |= basename == nsStr;
    }

    if (const auto* typeDecl = dyn_cast<TypeDecl>(namedDecl))
    {
      auto typeName = typeDecl->getNameAsString();
      convertString(typeName, namingMap.at(naming::Declaration::Type));
      foundAnyMatch |= basename == typeName;
    }
  }

  return foundAnyMatch;
}
} // namespace

BasenameDeclarationCheck::BasenameDeclarationCheck(StringRef Name,
                                                   ClangTidyContext* context)
    : ClangTidyCheck(Name, context),
      _typeCase{parseStringList(Options.get("TypeCase", "CamelCase"))},
      _namespaceCase{parseStringList(Options.get("NamespaceCase", "lower_case"))},
      _filenameCase{parseStringList(Options.get("FilenameCase", "lower_case"))}
{
  _declarationCaseMap[naming::Declaration::Type] =
      getCase(_typeCase).value_or(naming::Case::aNy_CasE);

  _declarationCaseMap[naming::Declaration::Namespace] =
      getCase(_namespaceCase).value_or(naming::Case::aNy_CasE);

  _declarationCaseMap[naming::Declaration::File] =
      getCase(_filenameCase).value_or(naming::Case::aNy_CasE);
}

void BasenameDeclarationCheck::storeOptions(ClangTidyOptions::OptionMap& Opts)
{
  using utils::options::serializeStringList;
  Options.store(Opts, "TypeCase", serializeStringList(_typeCase));
  Options.store(Opts, "NamespaceCase", serializeStringList(_namespaceCase));
  Options.store(Opts, "FilenameCase", serializeStringList(_filenameCase));
}

void BasenameDeclarationCheck::registerMatchers(MatchFinder* Finder)
{
  Finder->addMatcher(
      namedDecl(anyOf(namespaceDecl(), recordDecl(), functionDecl())).bind("named"),
      this);
}

void BasenameDeclarationCheck::check(const MatchFinder::MatchResult& Result)
{
  const auto* matchedDecl = Result.Nodes.getNodeAs<NamedDecl>("named");

  if (!Result.SourceManager->isInMainFile(matchedDecl->getLocation()))
  {
    return;
  }

  if (matchedDecl->getLocation().isMacroID())
  {
    return;
  }

  auto filename = Result.SourceManager->getFilename(matchedDecl->getLocation());
  auto file     = std::filesystem::path{filename.data()};

  if (file.extension() != ".hpp")
  {
    return;
  }

  _foundDeclarationsPerFile[file].emplace_back(matchedDecl);
}

// NOLINTNEXTLINE(readability-function-size)
void BasenameDeclarationCheck::onEndOfTranslationUnit()
{
  for (const auto& [filename, namedDecls] : _foundDeclarationsPerFile)
  {
    if (!findAnyMatch(filename, namedDecls, _declarationCaseMap))
    {
      diag("Did not find declaration with name matching basename '%0' of '%1'")
          << StringRef{filename.filename().c_str()} << filename.c_str();

      for (const auto& namedDecl : namedDecls)
      {
        diag("'%1': Found declaration '%0'", DiagnosticIDs::Note)
            << namedDecl->getNameAsString() << StringRef{filename.filename().c_str()};
      }
    }
  }
}

} // namespace clang::tidy::gfx
