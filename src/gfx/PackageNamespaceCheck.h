//===--- PackageNamespaceCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef GFX_CHECKS_SRC_GFX_PACKAGENAMESPACECHECK_H
#define GFX_CHECKS_SRC_GFX_PACKAGENAMESPACECHECK_H

#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/utils/OptionsUtils.h"

#include <string>

namespace clang {
namespace tidy {
namespace gfx {

class PackageNamespaceCheck : public ClangTidyCheck {
public:
  PackageNamespaceCheck(StringRef Name, ClangTidyContext *Context);

  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  std::string _filepath{};
  std::vector<StringRef> _allowed{};
};

} // namespace gfx
} // namespace tidy
} // namespace clang

#endif // GFX_CHECKS_SRC_GFX_PACKAGENAMESPACECHECK_H
