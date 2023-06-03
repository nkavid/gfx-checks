//===--- ImplementationInNamespaceCheck.h - clang-tidy ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#pragma once

#include <filesystem>
#include <map>
#include <string>

#include "clang-tidy/ClangTidyCheck.h"

namespace clang::tidy::gfx
{
namespace naming
{
enum class Declaration
{
  Type,
  Namespace,
  File
  // "Uhm ackshually 'file' is not a declaration"
  // You're a declaration. :(
};

enum class Case
{
  snake_case,
  camelCase,
  // "Uhm ackshually it's 'CamelCase' and 'camelBack' in docs"
  // You're a camel back. :(
  PascalCase,
  aNy_CasE,
};

using Map = std::map<naming::Declaration, naming::Case>;
} // namespace naming

class BasenameDeclarationCheck : public ClangTidyCheck
{
  public:
    BasenameDeclarationCheck(StringRef Name, ClangTidyContext* context);

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
    std::vector<StringRef> _typeCase;
    std::vector<StringRef> _namespaceCase;
    std::vector<StringRef> _filenameCase;
    naming::Map _declarationCaseMap{};
    std::map<std::filesystem::path, std::vector<const NamedDecl*>>
        _foundDeclarationsPerFile{};
};

} // namespace clang::tidy::gfx
