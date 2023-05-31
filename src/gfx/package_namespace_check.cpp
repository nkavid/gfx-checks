// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/package_namespace_check.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

#include <clang-tidy/utils/OptionsUtils.h>

using namespace clang::ast_matchers;

namespace clang::tidy::gfx
{

PackageNamespaceCheck::PackageNamespaceCheck(StringRef Name, ClangTidyContext* context)
    : ClangTidyCheck(Name, context),
      _allowed(utils::options::parseStringList(Options.get("Allowed", "detail")))
{
  auto string = context->getCurrentFile().str();
  auto loc    = string.find("gfx/");
  _filepath   = string.substr(loc);
}

void PackageNamespaceCheck::registerMatchers(MatchFinder* Finder)
{
  Finder->addMatcher(namespaceDecl().bind("namespace_declaration"), this);
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity,readability-function-size)
void PackageNamespaceCheck::check(const MatchFinder::MatchResult& Result)
{
  const auto* matchedDecl =
      Result.Nodes.getNodeAs<NamespaceDecl>("namespace_declaration");

  if (!Result.SourceManager->isInMainFile(matchedDecl->getLocation()))
  {
    return;
  }

  if (matchedDecl->isAnonymousNamespace())
  {
    return;
  }

  for (const auto& allowed : _allowed)
  {
    if (matchedDecl->getName() == allowed)
    {
      return;
    }
  }

  if (_filepath.find(matchedDecl->getName()) == std::string::npos)
  {
    if (const auto* parentNamespace = dyn_cast<NamespaceDecl>(matchedDecl->getParent()))
    {
      if (_filepath.find(parentNamespace->getName()) == std::string::npos)
      {
        if (parentNamespace->getName() == _allowed[0])
        {
          return;
        }
        diag(matchedDecl->getLocation(),
             "'%0' parent namespace for '%1' does "
             "not match any directories in '%2'")
            << parentNamespace->getName() << matchedDecl->getName() << _filepath;
      }
      else
      {
        return;
      }
    }
    diag(matchedDecl->getLocation(), "'%0' does not match any directories in '%1'")
        << matchedDecl->getName() << _filepath;
  }
}

} // namespace clang
