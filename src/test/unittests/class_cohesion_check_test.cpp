// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/class_cohesion_check.hpp"

#include "ClangTidyTest.h"
#include "gtest/gtest.h"

namespace clang
{
namespace tidy
{
namespace test
{
TEST(GFXModuleTest, ClassCohesion)
{
  using namespace clang::tidy::gfx;

  ClangTidyOptions Options;
  Options.CheckOptions["test-check-0.Score"] = "50";

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<ClassCohesionCheck>("class Foo\n"
                                     "{\n"
                                     "public:\n"
                                     "  int x() {return 5;}\n"
                                     "  int y() {return a;}\n"
                                     "  int z() {return b + a;}\n"
                                     "private:\n"
                                     "  int a{3};\n"
                                     "  int b{4};\n"
                                     "};\n"
                                     "\n"
                                     "int i{8};\n"
                                     "\n"
                                     "class Bar\n"
                                     "{\n"
                                     "public:\n"
                                     "  int y() {return a;}\n"
                                     "private:\n"
                                     "  int a{1};\n"
                                     "  int b{1};\n"
                                     "};\n",
                                     &Errors,
                                     "foo/bar.cpp",
                                     std::nullopt,
                                     Options);

  EXPECT_EQ(3U, Errors.size());
}

} // namespace test
} // namespace tidy
} // namespace clang
