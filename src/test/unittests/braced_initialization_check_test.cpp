// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/braced_initialization_check.hpp"

#include "ClangTidyTest.h"
#include "gtest/gtest.h"
#include "utils.hpp"

namespace clang
{
namespace tidy
{
namespace test
{
namespace
{
namespace dummy_source
{
const char* braced_init = "int intVar1;\n"
                          "unsigned long intVar2 = 2;\n"
                          "unsigned long intVar3 = 2.0f;\n"
                          "float floatVar = 0.8f;\n"
                          "int intVar4{};\n"
                          "\n";
} // namespace dummy_source
} // namespace

TEST(GFXModuleTest, PreferBracedInit)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<BracedInitializationCheck>(dummy_source::braced_init,
                                            &Errors,
                                            "dummyfilename.cpp");

  EXPECT_EQ(2U, Errors.size())
      << utils::getErrorString(Errors, dummy_source::braced_init);
}

} // namespace test
} // namespace tidy
} // namespace clang
