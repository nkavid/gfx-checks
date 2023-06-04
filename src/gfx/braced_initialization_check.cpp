// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/braced_initialization_check.hpp"

#include "llvm/Support/WithColor.h"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

#include <clang-tidy/utils/OptionsUtils.h>

namespace clang::tidy::gfx
{
namespace
{
std::string_view getInitStyleString(VarDecl::InitializationStyle initializationStyle)
{
  switch (initializationStyle)
  {
  case VarDecl::CInit:
    return "c-style initialization";
  case VarDecl::CallInit:
    return "call-style initialization";
  case VarDecl::ParenListInit:
    return "Parenthesized-list initialization";
  case VarDecl::ListInit:
    return "Direct list initialization";
  }
  return "NOT RECOGNIZED";
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
FixItHint getFixItHint(const VarDecl* varDecl)
{
  if (varDecl->getInitStyle() == VarDecl::CInit)
  {
    const auto* initExpr = varDecl->getInit();
    if (initExpr == nullptr)
    {
      const auto replacementString = varDecl->getNameAsString() + '{' + '}';
      return FixItHint::CreateReplacement(varDecl->getLocation(), replacementString);
    }

    const auto* boolExpr = dyn_cast<CXXBoolLiteralExpr>(initExpr);
    if (boolExpr != nullptr)
    {
      const auto replacementRange = SourceRange{varDecl->getLocation(),
                                                initExpr->getExprLoc()};
      const auto* boolStr         = boolExpr->getValue() ? "true" : "false";

      const auto replacementString = varDecl->getNameAsString() + '{' + boolStr + '}';
      return FixItHint::CreateReplacement(replacementRange, replacementString);
    }

    const auto* integerLiteral = dyn_cast<IntegerLiteral>(initExpr);
    if (integerLiteral != nullptr)
    {
      const auto replacementRange = SourceRange{varDecl->getLocation(),
                                                initExpr->getExprLoc()};

      const uint64_t integerValue = integerLiteral->getValue().getLimitedValue();
      const auto integerString    = std::to_string(integerValue);

      const auto replacementString = varDecl->getNameAsString() + '{' + integerString
                                   + '}';
      return FixItHint::CreateReplacement(replacementRange, replacementString);
    }

    const auto* implicitCastExpr = dyn_cast<ImplicitCastExpr>(initExpr);
    if (implicitCastExpr != nullptr)
    {
      varDecl->dumpColor();
      llvm::WithColor::error(llvm::errs(), "Implicit cast")
          << "Implement replacement with literal suffix\n\n";
    }
  }
  return FixItHint{};
}
} // namespace

BracedInitializationCheck::BracedInitializationCheck(StringRef Name,
                                                     ClangTidyContext* context)
    : ClangTidyCheck(Name, context)
{}

void BracedInitializationCheck::registerMatchers(ast_matchers::MatchFinder* Finder)
{
  using namespace ast_matchers;

  Finder->addMatcher(varDecl(unless(anyOf(parmVarDecl(),
                                          hasParent(cxxCatchStmt()),
                                          hasAncestor(forStmt()),
                                          hasAncestor(cxxForRangeStmt()),
                                          hasDescendant(cxxConstructExpr()),
                                          hasDescendant(declRefExpr()))))
                         .bind("vardecl"),
                     this);
}

void BracedInitializationCheck::check(
    const ast_matchers::MatchFinder::MatchResult& Result)
{
  const auto* varDecl = Result.Nodes.getNodeAs<VarDecl>("vardecl");
  if (!Result.SourceManager->isInMainFile(varDecl->getLocation()))
  {
    return;
  }

  auto initStyle = varDecl->getInitStyle();
  if (initStyle != VarDecl::ListInit)
  {
    diag(varDecl->getLocation(), "Variable '%0' has %1")
        << varDecl->getNameAsString() << getInitStyleString(initStyle)
        << getFixItHint(varDecl);
  }
}

} // namespace clang::tidy::gfx
