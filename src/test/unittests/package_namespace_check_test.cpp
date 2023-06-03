// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/package_namespace_check.hpp"

#include "ClangTidyTest.h"
#include "gtest/gtest.h"
#include "utils.hpp"

namespace clang
{
namespace tidy
{
namespace test
{
TEST(GFXModuleTest, AllowedOption)
{
  using namespace clang::tidy::gfx;

  ClangTidyOptions Options{};
  Options.CheckOptions["test-check-0.Allowed"] = "wow";

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<PackageNamespaceCheck>("namespace gfx\n"
                                        "{\n"
                                        "namespace foo\n"
                                        "{\n"
                                        "namespace wow\n"
                                        "{\n"
                                        "namespace utils\n"
                                        "{\n"
                                        "class Foo;\n"
                                        "}\n"
                                        "}\n"
                                        "}\n"
                                        "}\n",
                                        &Errors,
                                        "wow/gfx/foo/bar.cpp",
                                        std::nullopt,
                                        Options);

  EXPECT_EQ(0U, Errors.size()) << utils::getErrorString(Errors);
}

TEST(GFXModuleTest, IncorrectOrder)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<PackageNamespaceCheck>("namespace gfx::bar::foo\n"
                                        "{\n"
                                        "class Foo;\n"
                                        "}\n",
                                        &Errors,
                                        "gfx/foo/bar/baz.cpp");

  EXPECT_EQ(0U, Errors.size()) << utils::getErrorString(Errors);
}

} // namespace test
} // namespace tidy
} // namespace clang
