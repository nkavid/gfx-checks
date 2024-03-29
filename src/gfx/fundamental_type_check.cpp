// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/fundamental_type_check.hpp"

#include <clang-tidy/ClangTidyCheck.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/Type.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchersInternal.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceManager.h>

#include <string>

namespace clang::tidy::gfx
{
FundamentalTypeCheck::FundamentalTypeCheck(StringRef Name, ClangTidyContext* context)
    : ClangTidyCheck(Name, context)
{}

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
  if (matchedBuiltin != nullptr)
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
  if (matchedString != nullptr)
  {
    if (!Result.SourceManager->isInMainFile(matchedString->getLocation()))
    {
      return;
    }

    diag(matchedString->getLocation(), "Variable '%0' is a std::string")
        << matchedString->getNameAsString();
  }

  const auto* matchedCharPtr = Result.Nodes.getNodeAs<VarDecl>("char-ptr");
  if (matchedCharPtr != nullptr)
  {
    if (!Result.SourceManager->isInMainFile(matchedCharPtr->getLocation()))
    {
      return;
    }

    diag(matchedCharPtr->getLocation(), "Variable '%0' is a 'char*'")
        << matchedCharPtr->getNameAsString();
  }
}

} // namespace clang::tidy::gfx
