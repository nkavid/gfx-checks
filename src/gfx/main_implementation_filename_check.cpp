// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/main_implementation_filename_check.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

#include <clang-tidy/utils/OptionsUtils.h>

using namespace clang::ast_matchers;

namespace clang::tidy::gfx
{

MainImplementationFilenameCheck::MainImplementationFilenameCheck(
    StringRef Name,
    ClangTidyContext* context)
    : ClangTidyCheck(Name, context),
      _affixes(utils::options::parseStringList(Options.get("Affix", "none")))
{
}

void MainImplementationFilenameCheck::storeOptions(ClangTidyOptions::OptionMap& Opts)
{
  Options.store(Opts, "Affix", utils::options::serializeStringList(_affixes));
}

void MainImplementationFilenameCheck::registerMatchers(MatchFinder* Finder)
{
  Finder->addMatcher(functionDecl(isMain()).bind("main"), this);
}

void MainImplementationFilenameCheck::check(const MatchFinder::MatchResult& Result)
{
  const auto* MatchedDecl = Result.Nodes.getNodeAs<Decl>("main");

  if (!Result.SourceManager->isInMainFile(MatchedDecl->getLocation()))
  {
    return;
  }

  auto FileName = Result.SourceManager->getFilename(MatchedDecl->getLocation());

  if (std::find(_affixes.begin(), _affixes.end(), "none") != _affixes.end())
  {
    if (FileName.endswith("/main.cpp"))
    {
      return;
    }
  }

  if (std::find(_affixes.begin(), _affixes.end(), "suffix") != _affixes.end())
  {
    if (FileName.endswith("_main.cpp"))
    {
      return;
    }
  }

  diag(MatchedDecl->getLocation(), "main definition must be in file with 'main' affix");
}

} // namespace clang
