// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/class_cohesion_check.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>

#include <cmath>
#include <numeric>

#include <clang-tidy/utils/OptionsUtils.h>

using namespace clang::ast_matchers;

namespace clang::tidy::gfx
{
namespace
{
[[nodiscard]] size_t calcScore(size_t number, size_t total)
{
  const float ratio = static_cast<float>(number) / static_cast<float>(total);
  return static_cast<size_t>(std::round(100.0F * ratio));
}

[[nodiscard]] size_t calcAverage(std::vector<size_t> scores)
{
  auto sum = static_cast<float>(std::accumulate(scores.begin(), scores.end(), 0UL));
  return static_cast<size_t>(std::round(sum / static_cast<float>(scores.size())));
}

class UsedMemberVisitor : public RecursiveASTVisitor<UsedMemberVisitor>
{
  public:
    UsedMemberVisitor(const std::vector<const FieldDecl*>& fieldDecls)
    {
      for (const auto* elem : fieldDecls)
      {
        _fieldDeclsCount[elem] = false;
      }
    }

    bool VisitMemberExpr(MemberExpr* Node)
    {
      const auto* fieldDecl          = dyn_cast<FieldDecl>(Node->getMemberDecl());
      _fieldDeclsCount.at(fieldDecl) = true;
      return true;
    }

    [[nodiscard]] size_t getScore() const
    {
      size_t count{0};
      for (const auto& [_, isUsed] : _fieldDeclsCount)
      {
        count += isUsed ? 1 : 0;
      }
      return calcScore(count, _fieldDeclsCount.size());
    }

  private:
    std::map<const FieldDecl*, bool> _fieldDeclsCount{};
};

constexpr uint32_t defaultMaxAllowedScore{100};
} // namespace

ClassCohesionCheck::ClassCohesionCheck(StringRef Name, ClangTidyContext* context)
    : ClangTidyCheck(Name, context),
      _maxAllowedScore(Options.get("Score", defaultMaxAllowedScore))
{}

void ClassCohesionCheck::storeOptions(ClangTidyOptions::OptionMap& Opts)
{
  Options.store(Opts, "Score", _maxAllowedScore);
}

void ClassCohesionCheck::registerMatchers(MatchFinder* Finder)
{
  Finder->addMatcher(cxxMethodDecl().bind("method"), this);
  Finder->addMatcher(fieldDecl().bind("member"), this);
}

void ClassCohesionCheck::check(const MatchFinder::MatchResult& Result)
{
  auto addIfValid = [&Result](const char* matcher, auto& map) {
    using MapType     = std::remove_reference_t<decltype(map)>;
    using ParentType  = std::remove_pointer_t<typename MapType::key_type>;
    using VectorType  = std::remove_pointer_t<typename MapType::mapped_type>;
    using ElementType = std::remove_pointer_t<typename VectorType::value_type>;

    auto* match = Result.Nodes.getNodeAs<ElementType>(matcher);
    if (match == nullptr)
    {
      return;
    }

    if (!Result.SourceManager->isInMainFile(match->getLocation()))
    {
      return;
    }

    auto* parent = dyn_cast<ParentType>(match->getParent());
    if (parent != nullptr)
    {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      map[parent].emplace_back(match);
    }
  };

  addIfValid("method", _classMethods);
  addIfValid("member", _classMembers);
}

void ClassCohesionCheck::onEndOfTranslationUnit()
{
  for (const auto& [parent, methods] : _classMethods)
  {
    std::vector<size_t> scores{};
    for (const auto& method : methods)
    {
      UsedMemberVisitor visitor{_classMembers[parent]};
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
      visitor.TraverseDecl(const_cast<CXXMethodDecl*>(method));
      scores.emplace_back(visitor.getScore());
    }

    const size_t average = calcAverage(scores);
    if (average <= _maxAllowedScore)
    {
      reportDiagnostic(parent, average, methods, scores);
    }
  }
}

void ClassCohesionCheck::reportDiagnostic(
    const CXXRecordDecl* parent,
    size_t average,
    const std::vector<const CXXMethodDecl*>& methods,
    const std::vector<size_t>& scores)
{
  diag(parent->getLocation(), "%0 Average score %1")
      << parent->getNameAsString() << average;

  auto scoresIter = scores.begin();
  for (const auto& element : methods)
  {
    diag("%0::%1 Score %2", DiagnosticIDs::Note)
        << parent->getNameAsString() << element->getNameAsString() << *scoresIter;

    ++scoresIter;
  }
}

} // namespace clang::tidy::gfx
