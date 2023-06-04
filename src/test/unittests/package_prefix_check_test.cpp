// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "gfx/package_prefix_check.hpp"

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
const char* package_prefix = "void bar_foo_func(void);\n"
                             "void foo_br_func(void);\n"
                             "\n"
                             "struct fo_bar_data\n"
                             "{\n"
                             "int lorem;\n"
                             "int ipsum;\n"
                             "};\n"
                             "\n"
                             "typedef struct _foo_bar_thing\n"
                             "{\n"
                             "int lorem;\n"
                             "int ipsum;\n"
                             "} foobar_thing;\n"
                             "\n"
                             "enum oo_bar_name\n"
                             "{\n"
                             "fo_bar_lorem,\n"
                             "foo_br_ipsum\n"
                             "};\n";

} // namespace dummy_source
} // namespace

TEST(GFXModuleTest, PackagePrefix)
{
  using namespace clang::tidy::gfx;

  ClangTidyOptions Options{};

  std::vector<std::string> ExtraArgs{"-std=c2x"};
  std::vector<ClangTidyError> Errors{};
  runCheckOnCode<PackagePrefixCheck>(dummy_source::package_prefix,
                                     &Errors,
                                     "baz/foo/bar.h",
                                     ExtraArgs);

  EXPECT_EQ(7U, Errors.size()) << utils::getErrorString(Errors);
}

} // namespace test
} // namespace tidy
} // namespace clang
