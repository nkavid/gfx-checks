// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <clang-tidy/ClangTidyCheck.h>
#include <clang-tidy/ClangTidyDiagnosticConsumer.h>
#include <clang-tidy/ClangTidyOptions.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/LLVM.h>
#include <clang/Basic/LangOptions.h>
#include <llvm/ADT/StringRef.h>

#include <cstddef>
#include <map>
#include <vector>

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
    void reportDiagnostic(const CXXRecordDecl* parent,
                          size_t average,
                          const std::vector<const CXXMethodDecl*>& methods,
                          const std::vector<size_t>& scores);
    std::map<const CXXRecordDecl*, std::vector<const FieldDecl*>> _classMembers{};
    std::map<const CXXRecordDecl*, std::vector<const CXXMethodDecl*>> _classMethods{};

    unsigned _maxAllowedScore{};
};

} // namespace clang::tidy::gfx
