//===--- ImplementationInNamespaceCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef GFX_CHECKS_SRC_GFX_IMPLEMENTATIONINNAMESPACECHECK_H
#define GFX_CHECKS_SRC_GFX_IMPLEMENTATIONINNAMESPACECHECK_H

#include "ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace gfx {

/// Checks all llvm-libc implementation is within the correct namespace.
class ImplementationInNamespaceCheck : public ClangTidyCheck {
public:
  ImplementationInNamespaceCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace gfx
} // namespace tidy
} // namespace clang

#endif // GFX_CHECKS_SRC_GFX_IMPLEMENTATIONINNAMESPACECHECK_H
