//===--- FundamentalTypeCheck.cpp - clang-tidy
//------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "gfx/FundamentalTypeCheck.h"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

#include <clang-tidy/utils/OptionsUtils.h>

namespace clang::tidy::gfx
{
FundamentalTypeCheck::FundamentalTypeCheck(StringRef Name, ClangTidyContext* context)
    : ClangTidyCheck(Name, context)
{
}

void FundamentalTypeCheck::registerMatchers(ast_matchers::MatchFinder* Finder)
{
  using namespace ast_matchers;

  Finder->addMatcher(varDecl(hasType(qualType(hasCanonicalType(builtinType()))),
                             unless(hasName("argc")))
                         .bind("builtin"),
                     this);

  Finder->addMatcher(varDecl(hasType(elaboratedType(namesType(
                                 hasDeclaration(namedDecl(hasName("std::string")))))))
                         .bind("string"),
                     this);

  Finder->addMatcher(varDecl(hasType(qualType(pointerType(pointee(isAnyCharacter())))))
                         .bind("char-ptr"),
                     this);
}

void FundamentalTypeCheck::check(const ast_matchers::MatchFinder::MatchResult& Result)
{
  const auto* matchedBuiltin = Result.Nodes.getNodeAs<VarDecl>("builtin");
  if (matchedBuiltin)
  {
    if (!Result.SourceManager->isInMainFile(matchedBuiltin->getLocation()))
    {
      return;
    }

    diag(matchedBuiltin->getLocation(), "Variable '%0' has built-in type '%1'")
        << matchedBuiltin->getNameAsString()
        << matchedBuiltin->getType()
               .getCanonicalType()
               .getUnqualifiedType()
               .getAsString();
  }

  const auto* matchedString = Result.Nodes.getNodeAs<VarDecl>("string");
  if (matchedString)
  {
    if (!Result.SourceManager->isInMainFile(matchedString->getLocation()))
    {
      return;
    }

    diag(matchedString->getLocation(), "Variable '%0' is a std::string")
        << matchedString->getNameAsString();
  }

  const auto* matchedCharPtr = Result.Nodes.getNodeAs<VarDecl>("char-ptr");
  if (matchedCharPtr)
  {
    if (!Result.SourceManager->isInMainFile(matchedCharPtr->getLocation()))
    {
      return;
    }

    diag(matchedCharPtr->getLocation(), "Variable '%0' is a 'char*'")
        << matchedCharPtr->getNameAsString();
  }
}

} // namespace clang
