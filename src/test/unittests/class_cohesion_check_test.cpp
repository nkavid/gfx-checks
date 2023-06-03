// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/class_cohesion_check.hpp"

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
const char* dummy = "class Foo\n"
                    "{\n"
                    "public:\n"
                    "Foo() {}\n"
                    "\n"
                    "~Foo() = default;\n"
                    "\n"
                    "int x();\n"
                    "int y();\n"
                    "int z();\n"
                    "\n"
                    "private:\n"
                    "int _a{3};\n"
                    "int _b{4};\n"
                    "};\n"
                    "\n"
                    "int Foo::x()\n"
                    "{\n"
                    "return 5;\n"
                    "}\n"
                    "\n"
                    "int Foo::y()\n"
                    "{\n"
                    "return _a;\n"
                    "}\n"
                    "\n"
                    "int Foo::z()\n"
                    "{\n"
                    "return _b + _a;\n"
                    "}\n";

} // namespace dummy_source
} // namespace

TEST(GFXModuleTest, ClassCohesion)
{
  using namespace clang::tidy::gfx;

  ClangTidyOptions Options;
  Options.CheckOptions["test-check-0.Score"] = "100";

  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<ClassCohesionCheck>(dummy_source::dummy,
                                     &Errors,
                                     "foo/bar.cpp",
                                     std::nullopt,
                                     Options);

  auto errString = utils::getErrorString(Errors);

  EXPECT_EQ(1U, Errors.size()) << errString;
  EXPECT_TRUE(errString.find("score 50") != std::string::npos) << errString;
}

} // namespace test
} // namespace tidy
} // namespace clang
