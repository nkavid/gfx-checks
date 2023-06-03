// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/main_implementation_filename_check.hpp"

#include "ClangTidyTest.h"
#include "gtest/gtest.h"

namespace clang
{
namespace tidy
{
namespace test
{
TEST(GFXModuleTest, WrongFileName)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<MainImplementationFilenameCheck>("int main() {}",
                                                  &Errors,
                                                  "foo/bar.cpp");

  EXPECT_EQ("main definition must be in file with 'main' affix",
            Errors[0].Message.Message);
}

TEST(GFXModuleTest, MainFileNameDefault)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<MainImplementationFilenameCheck>("int main() {}",
                                                  &Errors,
                                                  "foo/main.cpp");

  EXPECT_EQ(0U, Errors.size());
}

TEST(GFXModuleTest, MainSuffixOption)
{
  using namespace clang::tidy::gfx;

  ClangTidyOptions Options;
  Options.CheckOptions["test-check-0.Affix"] = "wow;suffix";

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<MainImplementationFilenameCheck>("int main() {}",
                                                  &Errors,
                                                  "foo/bar_main.cpp",
                                                  std::nullopt,
                                                  Options);

  EXPECT_EQ(0U, Errors.size());
}

} // namespace test
} // namespace tidy
} // namespace clang
