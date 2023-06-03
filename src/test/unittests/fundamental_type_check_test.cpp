// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/fundamental_type_check.hpp"

#include "ClangTidyTest.h"
#include "gtest/gtest.h"
#include "utils.hpp"

namespace clang
{
namespace tidy
{
namespace test
{
TEST(GFXModuleTest, DisallowTiredMemes)
{
  using namespace clang::tidy::gfx;

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<FundamentalTypeCheck>("typedef unsigned int uint32_t;\n"
                                       "namespace std { class string{}; };\n"
                                       "uint32_t intVar;\n"
                                       "const std::string stringVar;\n"
                                       "const char* charPtr;\n",
                                       &Errors,
                                       "dummyfilename.cpp");

  EXPECT_EQ(3U, Errors.size()) << utils::getErrorString(Errors);
}

} // namespace test
} // namespace tidy
} // namespace clang
