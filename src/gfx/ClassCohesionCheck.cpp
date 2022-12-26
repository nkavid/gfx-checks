//===--- ClassCohesionCheck.cpp - clang-tidy
//------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "ClassCohesionCheck.h"
#include "utils/OptionsUtils.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace gfx {

ClassCohesionCheck::ClassCohesionCheck(StringRef Name,
                                       ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), Score(Options.get("Score", -1)) {}

void ClassCohesionCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "Score", Score);
}

void ClassCohesionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(tagDecl(isClass()).bind("class"), this);
  Finder->addMatcher(fieldDecl().bind("member"), this);
  Finder->addMatcher(cxxMethodDecl().bind("method"), this);
}

void ClassCohesionCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedTypeDecl = Result.Nodes.getNodeAs<TypeDecl>("class");
  if (MatchedTypeDecl) {
    if (Result.SourceManager->isInMainFile(MatchedTypeDecl->getLocation())) {
      Classes.push_back(MatchedTypeDecl);
    }
  }

  const auto *MatchedNamedDecl = Result.Nodes.getNodeAs<FieldDecl>("member");
  if (MatchedNamedDecl) {
    if (Result.SourceManager->isInMainFile(MatchedNamedDecl->getLocation())) {
      Members.push_back(MatchedNamedDecl);
    }
  }

  const auto *MatchedMethodDecl =
      Result.Nodes.getNodeAs<FunctionDecl>("method");
  if (MatchedMethodDecl) {
    if (Result.SourceManager->isInMainFile(MatchedMethodDecl->getLocation())) {
      Methods.push_back(MatchedMethodDecl);
    }
  }
}

void ClassCohesionCheck::onEndOfTranslationUnit() {
  for (const auto *elem : Classes) {
    diag(elem->getLocation(), "Classes: %0") << elem->getName();
  }

  for (const auto *elem : Members) {
    diag(elem->getLocation(), "Members: %0, %1")
        << elem->getName() << elem->getParent()->getName();
  }

  for (const auto *elem : Methods) {
    diag(elem->getLocation(), "Methods: %0") << elem->getName();
  }
}

} // namespace gfx
} // namespace tidy
} // namespace clang
