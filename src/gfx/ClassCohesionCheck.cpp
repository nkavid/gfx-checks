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
    : ClangTidyCheck(Name, Context), Score(Options.get("Score", 100)) {}

void ClassCohesionCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
  Options.store(Opts, "Score", Score);
}

void ClassCohesionCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(cxxMethodDecl().bind("method"), this);
}

void ClassCohesionCheck::check(const MatchFinder::MatchResult &Result) {
  auto addIfValid = [&Result](const char *matcher, auto &vector) {
    using VectorType = std::remove_reference_t<decltype(vector)>;
    using ElementType = std::remove_pointer_t<typename VectorType::value_type>;

    const auto *match = Result.Nodes.getNodeAs<ElementType>(matcher);
    if (match) {
      if (Result.SourceManager->isInMainFile(match->getLocation())) {
        vector.push_back(match);
      }
    }
  };

  addIfValid("method", Methods);
}

static void traverse(const TypeDecl *parent, const Stmt *statement,
                     std::vector<const FieldDecl *> &members) {
  if (!statement) {
    return;
  }
  if (!parent) {
    return;
  }
  const auto children = statement->children();
  if (children.empty()) {
    return;
  }

  for (const auto &child : children) {
    if (!child) {
      continue;
    }
    if (child->getStmtClass() == Stmt::MemberExprClass) {
      const auto *member = dyn_cast<MemberExpr>(child)->getMemberDecl();
      if (dyn_cast<FieldDecl>(member)) {
        if (dyn_cast<FieldDecl>(member)->getParent() == parent) {
          members.push_back(dyn_cast<FieldDecl>(member));
        }
      }
    }
    traverse(parent, child, members);
  }
}

void ClassCohesionCheck::onEndOfTranslationUnit() {
  for (const auto *method : Methods) {
    const auto *parent = dyn_cast<TypeDecl>(method->getParent());
    allClassMethods[parent].push_back(method);

    const auto *methodBody = method->getBody();
    auto &members = usedMembers[method];
    traverse(parent, methodBody, members);

    std::unique(members.begin(), members.end());
  }

  for (const auto &[method, members] : usedMembers) {
    const auto *parent = dyn_cast<TypeDecl>(method->getParent());

    auto &allMembers = allClassMembers[parent];

    allMembers.insert(allMembers.end(), members.begin(), members.end());
    std::unique(allMembers.begin(), allMembers.end());
  }

  for (const auto &[checkedClass, allMembers] : allClassMembers) {
    const auto methods = allClassMethods[checkedClass];

    unsigned numMembers = allMembers.size();
    unsigned sum = 0;
    for (const auto method : methods) {
      unsigned numUsed = usedMembers[method].size();
      auto score =
          100.0f * static_cast<float>(numUsed) / static_cast<float>(numMembers);
      methodScores[method] = static_cast<unsigned>(score);
      sum += methodScores[method];
    }

    sum = sum / methods.size();

    if (sum <= Score) {
      for (const auto *method : methods) {
        diag(method->getLocation(), "[%0 total %3] %0::%1() score %2")
            << dyn_cast<NamedDecl>(method->getParent())->getName()
            << method->getName() << methodScores[method] << sum;
      }
    }
  }
}

} // namespace gfx
} // namespace tidy
} // namespace clang
