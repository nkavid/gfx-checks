// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/package_prefix_check.hpp"

#include <clang-tidy/ClangTidyCheck.h>
#include <clang/AST/Decl.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchersInternal.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/SourceManager.h>
#include <llvm/ADT/Twine.h>

#include <tuple>

namespace clang::tidy::gfx
{
namespace
{
using namespace clang::ast_matchers;

std::tuple<StringRef, StringRef> getPackageNames(StringRef filename)
{
  const auto& [first, second] = filename.rsplit('/');

  const auto library   = first.substr(first.rfind('/') + 1);
  const auto component = second.substr(0, second.find('.'));

  return std::make_tuple(library, component);
}
} // namespace

PackagePrefixCheck::PackagePrefixCheck(StringRef Name, ClangTidyContext* context)
    : ClangTidyCheck(Name, context)
{}

void PackagePrefixCheck::registerMatchers(MatchFinder* Finder)
{
  Finder->addMatcher(namedDecl(unless(fieldDecl())).bind("named-c-decl"), this);
}

void PackagePrefixCheck::check(const MatchFinder::MatchResult& Result)
{
  const auto* matchedDecl = Result.Nodes.getNodeAs<NamedDecl>("named-c-decl");

  if (!Result.SourceManager->isInMainFile(matchedDecl->getLocation()))
  {
    return;
  }

  auto filename = Result.SourceManager->getFilename(matchedDecl->getLocation());
  if (!filename.ends_with(".h"))
  {
    return;
  }

  const auto [library, component] = getPackageNames(filename);

  const auto& declName = matchedDecl->getName();

  auto diagnostic = [this, &matchedDecl](const char* expectation,
                                         const StringRef& name) {
    diag(matchedDecl->getLocation(), expectation) << name;
  };

  if (declName.starts_with("_"))
  {
    if (!declName.contains(library))
    {
      diagnostic("Expected private declaration contains '%0'", library);
    }

    if (!declName.contains(component))
    {
      diagnostic("Expected private declaration contains '%0'", component);
    }

    return;
  }

  if (!declName.starts_with(llvm::Twine{library + "_"}.str()))
  {
    diagnostic("Expected declaration name starts with '%0_'", library);
  }

  if (!declName.contains(component))
  {
    diagnostic("Expected declaration contains '%0'", component);
  }
}

} // namespace clang::tidy::gfx
