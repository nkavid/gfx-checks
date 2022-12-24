//===--- ImplementationInNamespaceCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MainDefinedInFileWithMainAffixCheck.h"
#include "utils/OptionsUtils.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace gfx {

MainDefinedInFileWithMainAffixCheck::MainDefinedInFileWithMainAffixCheck(
    StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context),
      Affixes(utils::options::parseStringList(Options.get("Affix", "none"))) {}

void MainDefinedInFileWithMainAffixCheck::storeOptions(
    ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "Affix", utils::options::serializeStringList(Affixes));
}

void MainDefinedInFileWithMainAffixCheck::registerMatchers(
    MatchFinder *Finder) {
  Finder->addMatcher(functionDecl(isMain()).bind("main"), this);
}

void MainDefinedInFileWithMainAffixCheck::check(
    const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<Decl>("main");

  if (!Result.SourceManager->isInMainFile(MatchedDecl->getLocation())) {
    return;
  }

  auto FileName = Result.SourceManager->getFilename(MatchedDecl->getLocation());

  if (std::find(Affixes.begin(), Affixes.end(), "none") != Affixes.end()) {
    if (FileName.endswith("/main.cpp")) {
      return;
    }
  }

  if (std::find(Affixes.begin(), Affixes.end(), "suffix") != Affixes.end()) {
    if (FileName.endswith("_main.cpp")) {
      return;
    }
  }

  diag(MatchedDecl->getLocation(),
       "main definition must be in file with 'main' affix");
}

} // namespace gfx
} // namespace tidy
} // namespace clang
