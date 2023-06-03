// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/basename_declaration_check.hpp"

#include "ClangTidyTest.h"
#include "gtest/gtest.h"
#include "utils.hpp"

namespace clang
{
namespace tidy
{
namespace test
{
TEST(GFXModuleTest, ReportOnceIfNoMatch)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<BasenameDeclarationCheck>("namespace gfx\n"
                                           "{\n"
                                           "namespace this_is_foo\n"
                                           "{\n"
                                           "class ThisIsFoo;\n"
                                           "}\n"
                                           "}\n",
                                           &Errors,
                                           "foo/this_is_bar.hpp");

  EXPECT_EQ(1U, Errors.size()) << utils::getErrorString(Errors);
}

TEST(GFXModuleTest, CheckForClass)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<BasenameDeclarationCheck>("namespace gfx\n"
                                           "{\n"
                                           "namespace foo\n"
                                           "{\n"
                                           "class ThisIsBar;\n"
                                           "}\n"
                                           "}\n",
                                           &Errors,
                                           "foo/this_is_bar.hpp");

  EXPECT_EQ(0U, Errors.size()) << utils::getErrorString(Errors);
}

TEST(GFXModuleTest, CheckForNamespace)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<BasenameDeclarationCheck>("namespace gfx\n"
                                           "{\n"
                                           "namespace this_is_bar\n"
                                           "{\n"
                                           "class Foo;\n"
                                           "}\n"
                                           "}\n",
                                           &Errors,
                                           "foo/this_is_bar.hpp");

  EXPECT_EQ(0U, Errors.size()) << utils::getErrorString(Errors);
}

TEST(GFXModuleTest, AcceptDuplication)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<BasenameDeclarationCheck>("namespace gfx\n"
                                           "{\n"
                                           "namespace this_is_bar\n"
                                           "{\n"
                                           "class ThisIsBar;\n"
                                           "}\n"
                                           "}\n",
                                           &Errors,
                                           "foo/this_is_bar.hpp");

  EXPECT_EQ(0U, Errors.size()) << utils::getErrorString(Errors);
}

TEST(GFXModuleTest, SupportConfigurableNamingCases)
{
  using namespace clang::tidy::gfx;

  ClangTidyOptions Options{};
  Options.CheckOptions["test-check-0.TypeCase"]      = "lower_case";
  Options.CheckOptions["test-check-0.NamespaceCase"] = "camelBack";
  Options.CheckOptions["test-check-0.FilenameCase"]  = "CamelCase";

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<BasenameDeclarationCheck>("namespace gfx\n"
                                           "{\n"
                                           "namespace thisIsFoo\n"
                                           "{\n"
                                           "class this_is_foo;\n"
                                           "}\n"
                                           "}\n",
                                           &Errors,
                                           "foo/ThisIsFoo.hpp",
                                           std::nullopt,
                                           Options);

  EXPECT_EQ(0U, Errors.size()) << utils::getErrorString(Errors);
}

} // namespace test
} // namespace tidy
} // namespace clang
