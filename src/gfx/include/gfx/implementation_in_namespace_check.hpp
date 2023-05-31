//===--- ImplementationInNamespaceCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#pragma once

#include "clang-tidy/ClangTidyCheck.h"

namespace clang::tidy::gfx
{

class ImplementationInNamespaceCheck : public ClangTidyCheck
{
  public:
    ImplementationInNamespaceCheck(StringRef name, ClangTidyContext* context)
        : ClangTidyCheck(name, context)
    {
    }

    [[nodiscard]] bool
    isLanguageVersionSupported(const LangOptions& LangOpts) const override
    {
      return LangOpts.CPlusPlus;
    }

    void registerMatchers(ast_matchers::MatchFinder* Finder) override;
    void check(const ast_matchers::MatchFinder::MatchResult& Result) override;
};

} // namespace clang
