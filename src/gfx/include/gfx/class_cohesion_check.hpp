//===--- ClassCohesionCheck.h - clang-tidy ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#pragma once

#include <map>
#include <vector>

#include "clang-tidy/ClangTidyCheck.h"

namespace clang::tidy::gfx
{

class ClassCohesionCheck : public ClangTidyCheck
{
  public:
    ClassCohesionCheck(StringRef Name, ClangTidyContext* context);

    void storeOptions(ClangTidyOptions::OptionMap& Opts) override;

    [[nodiscard]] bool
    isLanguageVersionSupported(const LangOptions& LangOpts) const override
    {
      return LangOpts.CPlusPlus;
    }

    void registerMatchers(ast_matchers::MatchFinder* Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult& Result) override;
    void onEndOfTranslationUnit() override;

  private:
    std::map<const TypeDecl*, std::vector<const FieldDecl*>> _allClassMembers{};
    std::map<const TypeDecl*, std::vector<const FunctionDecl*>> _allClassMethods{};
    std::vector<const FunctionDecl*> _methods{};
    std::map<const FunctionDecl*, std::vector<const FieldDecl*>> _usedMembers{};
    std::map<const FunctionDecl*, unsigned> _methodScores{};
    unsigned _maxAllowedScore{};
};

} // namespace clang::tidy::gfx
