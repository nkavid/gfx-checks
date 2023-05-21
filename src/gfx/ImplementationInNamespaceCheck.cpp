//===--- ImplementationInNamespaceCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "gfx/ImplementationInNamespaceCheck.h"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace clang::ast_matchers;

namespace clang::tidy::gfx
{

constexpr static StringRef g_RequiredNamespace{"gfx"};

void ImplementationInNamespaceCheck::registerMatchers(MatchFinder* Finder)
{
  Finder->addMatcher(decl(hasParent(translationUnitDecl()), unless(linkageSpecDecl()))
                         .bind("child_of_translation_unit"),
                     this);
}

void ImplementationInNamespaceCheck::check(const MatchFinder::MatchResult& Result)
{
  const auto* matchedDecl = Result.Nodes.getNodeAs<Decl>("child_of_translation_unit");

  if (!Result.SourceManager->isInMainFile(matchedDecl->getLocation()))
  {
    return;
  }

  if (const auto* namespaceDecl = dyn_cast<NamespaceDecl>(matchedDecl))
  {
    if (namespaceDecl->getName() != g_RequiredNamespace)
    {
      diag(namespaceDecl->getLocation(), "'%0' needs to be the outermost namespace")
          << g_RequiredNamespace;
    }
    return;
  }

  if (const auto* namedDecl = dyn_cast<NamedDecl>(matchedDecl))
  {
    if (namedDecl->getName() == "main")
    {
      return;
    }
  }

  diag(matchedDecl->getLocation(),
       "declaration must be declared within the '%0' namespace")
      << g_RequiredNamespace;
}

} // namespace clang
