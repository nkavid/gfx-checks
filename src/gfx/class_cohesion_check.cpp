// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/class_cohesion_check.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

#include <clang-tidy/utils/OptionsUtils.h>

using namespace clang::ast_matchers;

namespace clang::tidy::gfx
{
constexpr uint32_t defaultMaxAllowedScore{100};

ClassCohesionCheck::ClassCohesionCheck(StringRef Name, ClangTidyContext* context)
    : ClangTidyCheck(Name, context),
      _maxAllowedScore(Options.get("Score", defaultMaxAllowedScore))
{
}

void ClassCohesionCheck::storeOptions(ClangTidyOptions::OptionMap& Opts)
{
  Options.store(Opts, "Score", _maxAllowedScore);
}

void ClassCohesionCheck::registerMatchers(MatchFinder* Finder)
{
  Finder->addMatcher(cxxMethodDecl().bind("method"), this);
}

// NOLINTNEXTLINE(readability-function-size)
void ClassCohesionCheck::check(const MatchFinder::MatchResult& Result)
{
  auto addIfValid = [&Result](const char* matcher, auto& vector) {
    using VectorType  = std::remove_reference_t<decltype(vector)>;
    using ElementType = std::remove_pointer_t<typename VectorType::value_type>;

    const auto* match = Result.Nodes.getNodeAs<ElementType>(matcher);
    if (match)
    {
      if (Result.SourceManager->isInMainFile(match->getLocation()))
      {
        vector.push_back(match);
      }
    }
  };

  addIfValid("method", _methods);
}

// NOLINTNEXTLINE
static void traverse(const TypeDecl* parent,
                     const Stmt* statement,
                     std::vector<const FieldDecl*>& members)
{
  if (statement == nullptr)
  {
    return;
  }

  if (parent == nullptr)
  {
    return;
  }

  const auto children = statement->children();
  if (children.empty())
  {
    return;
  }

  for (auto&& child : children)
  {
    if (child == nullptr)
    {
      continue;
    }

    if (child->getStmtClass() == Stmt::MemberExprClass)
    {
      const auto* member = dyn_cast<MemberExpr>(child)->getMemberDecl();
      if (dyn_cast<FieldDecl>(member) != nullptr)
      {
        if (dyn_cast<FieldDecl>(member)->getParent() == parent)
        {
          members.push_back(dyn_cast<FieldDecl>(member));
        }
      }
    }

    traverse(parent, child, members);
  }
}

// NOLINTNEXTLINE(readability-function-size)
void ClassCohesionCheck::onEndOfTranslationUnit()
{
  for (const auto* method : _methods)
  {
    const auto* parent = dyn_cast<TypeDecl>(method->getParent());
    _allClassMethods[parent].push_back(method);

    const auto* methodBody = method->getBody();
    auto& members          = _usedMembers[method];
    traverse(parent, methodBody, members);

    std::sort(members.begin(), members.end());
    auto lastIter = std::unique(members.begin(), members.end());
    members.erase(lastIter, members.end());
  }

  for (const auto& [method, members] : _usedMembers)
  {
    const auto* parent = dyn_cast<TypeDecl>(method->getParent());

    auto& allMembers = _allClassMembers[parent];
    allMembers.insert(allMembers.end(), members.begin(), members.end());

    std::sort(allMembers.begin(), allMembers.end());
    auto lastIter = std::unique(allMembers.begin(), allMembers.end());
    allMembers.erase(lastIter, allMembers.end());
  }

  for (const auto& [checkedClass, allMembers] : _allClassMembers)
  {
    const auto methods = _allClassMethods[checkedClass];

    auto numMembers = allMembers.size();
    size_t sum{0};
    for (const auto* const method : methods)
    {
      auto numUsed = _usedMembers[method].size();
      auto score   = 100.0F * static_cast<float>(numUsed)
                 / static_cast<float>(numMembers);
      _methodScores[method] = static_cast<unsigned>(score);
      sum += _methodScores[method];
    }

    sum = sum / methods.size();

    if (sum <= _maxAllowedScore)
    {
      for (const auto* method : methods)
      {
        diag(method->getLocation(), "[%0 total %3] %0::%1() score %2")
            << dyn_cast<NamedDecl>(method->getParent())->getName()
            << method->getNameInfo().getAsString() << _methodScores[method] << sum;
      }
    }
  }
}

} // namespace clang
