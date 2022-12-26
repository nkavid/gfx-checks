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
  auto addIfValid = [&Result](const char * matcher, auto& vector){
    using VectorType = std::remove_reference_t<decltype(vector)>;
    using ElementType = std::remove_pointer_t<typename VectorType::value_type>;

    const auto* match = Result.Nodes.getNodeAs<ElementType>(matcher);
    if (match) {
      if (Result.SourceManager->isInMainFile(match->getLocation())) {
        vector.push_back(match);
      }
    }
  };

  addIfValid("class", Classes);
  addIfValid("member", Members);
  addIfValid("method", Methods);
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
