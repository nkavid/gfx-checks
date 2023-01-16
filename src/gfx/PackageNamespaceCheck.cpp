//===--- PackageNamespaceCheck.cpp - clang-tidy ------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "PackageNamespaceCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace gfx {

void PackageNamespaceCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(namespaceDecl().bind("namespace_declaration"), this);
}

void PackageNamespaceCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl =
      Result.Nodes.getNodeAs<NamespaceDecl>("namespace_declaration");

  if (!Result.SourceManager->isInMainFile(MatchedDecl->getLocation())) {
    return;
  }

  if (MatchedDecl->getName() == "detail") {
    return;
  }

  if (MatchedDecl->isAnonymousNamespace()) {
    return;
  }

  auto FileName =
      Result.SourceManager->getFilename(MatchedDecl->getLocation()).str();

  auto RelativeFilename =
      FileName.substr(FileName.find("gfx"), std::string::npos);

  if (RelativeFilename.find(MatchedDecl->getName()) == std::string::npos) {
    if (const auto *parentNamespace =
            dyn_cast<NamespaceDecl>(MatchedDecl->getParent())) {
      if (RelativeFilename.find(parentNamespace->getName()) ==
          std::string::npos) {
        diag(MatchedDecl->getLocation(), "'%0' parent namespace for '%1' does "
                                         "not match any directories in '%2'")
            << parentNamespace->getName() << MatchedDecl->getName()
            << RelativeFilename;
      } else {
        return;
      }
    }
    diag(MatchedDecl->getLocation(),
         "'%0' does not match any directories in '%1'")
        << MatchedDecl->getName() << RelativeFilename;
  }
}

} // namespace gfx
} // namespace tidy
} // namespace clang
